#include "backends/vulkan/vulkan-renderer.hpp"

#include "backends/vulkan/vulkan-common.hpp"
#include "backends/vulkan/vulkan-pipeline.hpp"
#include "context.hpp"

#include <VkBootstrap.h>
#include <vector>

#ifndef _NDEBUG
#	define SHADER_PRINTF_ENABLE 1
#	define VALIDATION_LAYER_ENABLE 1
#else
#	define SHADER_PRINTF_ENABLE 0
#	define VALIDATION_LAYER_ENABLE 0
#endif
namespace aby::rhi::vulkan {

	auto Renderer::init(void* native_window) -> bool {
		auto& ctx = Context::get();
		auto* log = ctx.logger();

		if (!init_vulkan(native_window)) {
			aby_rhi_err("failed to init vulkan");
			return false;
		}

		if (!recreate_swapchain()) {
			aby_rhi_err("failed to create swapchain");
			return false;
		}

		if (!init_commands()) {
			aby_rhi_err("failed to init commands");
			return false;
		}

		if (!init_vma()) {
			aby_rhi_err("failed to init vulkan memory allocator");
			return false;
		}

		if (!init_draw_image()) {
			aby_rhi_err("failed to init draw image");
			return false;
		}

		if (!init_descriptors()) {
			aby_rhi_err("failed to init descriptors");
			return false;
		}

		return true;
	}

	auto Renderer::init_vulkan(void* native_window) -> bool {
		auto& ctx = Context::get();
		auto* log = ctx.logger();

		std::vector<const char*> instance_extensions, device_extensions;
		if (!get_extensions(&instance_extensions, &device_extensions)) {
			return false;
		}

		aby_rhi_dbg("[vulkan] instance extensions: {}", instance_extensions.size());
		for (size_t i = 0; i < instance_extensions.size(); i++) {
			aby_rhi_dbg("[vulkan] -- {}) {}", i, instance_extensions[i]);
		}

		vkb::InstanceBuilder instb;
		auto inst_ret = instb.set_app_name("aby-rhi")
		                    .set_allocation_callbacks(allocator())
		                    .set_debug_callback(vk_debug_callback)
		                    .set_debug_messenger_severity(VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		                                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
		                                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT)
		                    .request_validation_layers(VALIDATION_LAYER_ENABLE)
#if SHADER_PRINTF_ENABLE == 1
		                    .add_validation_feature_enable(VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT)
#endif
		                    .enable_extensions(instance_extensions)
		                    .require_api_version(1, 4, 3)
		                    .build();

		vkbcheck(inst_ret, "failed to create vulkan instance");

		m_Instance = inst_ret.value();

		VkSurfaceKHR surface = VK_NULL_HANDLE;
		if (!create_surface(native_window, m_Instance, &surface)) {
			log->log(ELogLevel::error, "[vulkan] failed to create surface");
			return false;
		}
		m_Surface = surface;

		vk::PhysicalDeviceVulkan11Features features11;
#if SHADER_PRINTF_ENABLE == 1
		features11.setStorageBuffer16BitAccess(vk::True);
#endif
		vk::PhysicalDeviceVulkan12Features features12;
		features12.setBufferDeviceAddress(vk::True)
		    .setDescriptorIndexing(vk::True)
		    .setRuntimeDescriptorArray(vk::True)
		    .setShaderSampledImageArrayNonUniformIndexing(vk::True)
		    .setDescriptorBindingPartiallyBound(vk::True)
		    .setDescriptorBindingSampledImageUpdateAfterBind(vk::True)
		    .setDescriptorBindingVariableDescriptorCount(vk::True)
#if SHADER_PRINTF_ENABLE == 1
		    .setVulkanMemoryModel(vk::True)
		    .setVulkanMemoryModelDeviceScope(vk::True)
		    .setScalarBlockLayout(vk::True)
		    .setStorageBuffer8BitAccess(vk::True)
		    .setShaderInt8(vk::True)
#endif
		    .setTimelineSemaphore(vk::True);

		vk::PhysicalDeviceVulkan13Features features13;
		features13.setDynamicRendering(vk::True)
		    .setSynchronization2(vk::True);

		vk::PhysicalDeviceVulkan14Features features14;

		vk::PhysicalDeviceFeatures features;
#if SHADER_PRINTF_ENABLE == 1
		features.setFragmentStoresAndAtomics(vk::True)
		    .setVertexPipelineStoresAndAtomics(vk::True)
		    .setShaderInt64(vk::True)
		    .setShaderInt16(vk::True);
#endif

		vkb::PhysicalDeviceSelector selector(inst_ret.value());
		auto gpu_ret = selector
		                   .set_minimum_version(1, 3)
		                   .set_required_features_14(features14)
		                   .set_required_features_13(features13)
		                   .set_required_features_12(features12)
		                   .set_required_features_11(features11)
		                   .set_required_features(features)
		                   .set_surface(m_Surface)
		                   .add_required_extensions(device_extensions)
		                   .select();

		vkbcheck(gpu_ret, "failed to find suitable gpu");

		vkb::DeviceBuilder dev_builder{ gpu_ret.value() };
		auto device_ret = dev_builder
		                      .set_allocation_callbacks(allocator())
		                      .build();

		vkbcheck(device_ret, "failed to create device");

		m_Device              = device_ret.value();
		m_GraphicsQueue       = m_Device.get_queue(vkb::QueueType::graphics).value();
		m_GraphicsQueueFamily = m_Device.get_queue_index(vkb::QueueType::graphics).value();
		m_PresentQueue        = m_Device.get_queue(vkb::QueueType::present).value();
		m_PresentQueueFamily  = m_Device.get_queue_index(vkb::QueueType::present).value();

		VkPhysicalDeviceProperties props = { 0 };
		vkGetPhysicalDeviceProperties(m_Device.physical_device, &props);
		aby_rhi_dbg("[vulkan] found GPU: {}", props.deviceName);
		return true;
	}

	auto Renderer::init_commands() -> bool {
		vk::CommandPoolCreateInfo command_pool_ci(
		    vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
		    m_GraphicsQueueFamily);

		for (size_t i = 0; i < m_Frames.size(); i++) {
			vkcheck(vkCreateCommandPool(
			            m_Device.device,
			            vkcast(command_pool_ci),
			            allocator(),
			            vkcast(m_Frames[i].pool)),
			        "failed to create command pool");

			vk::CommandBufferAllocateInfo cmd_alloc_info(m_Frames[i].pool, vk::CommandBufferLevel::ePrimary, 1);

			vkcheck(vkAllocateCommandBuffers(
			            m_Device.device,
			            vkcast(cmd_alloc_info),
			            vkcast(m_Frames[i].cmd)),
			        "failed to create command pool");

			vk::FenceCreateInfo fence_ci(vk::FenceCreateFlagBits::eSignaled);
			vkcheck(vkCreateFence(
			            m_Device.device,
			            vkcast(fence_ci),
			            allocator(),
			            vkcast(m_Frames[i].render_fence)),
			        "failed to create render fence");

			vk::SemaphoreCreateInfo semaphore_ci;
			vkcheck(vkCreateSemaphore(
			            m_Device.device,
			            vkcast(semaphore_ci),
			            allocator(),
			            vkcast(m_Frames[i].acquire)),
			        "failed to create wait semaphore");
		}

		return true;
	}

	auto Renderer::init_vma() -> bool {
		VmaAllocatorCreateInfo alloc_info = {};
		alloc_info.physicalDevice         = m_Device.physical_device;
		alloc_info.device                 = m_Device.device;
		alloc_info.instance               = m_Instance;
		alloc_info.flags                  = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
		vkcheck(vmaCreateAllocator(&alloc_info, &m_VMA), "failed to create VMA allocator");

		return true;
	}

	auto Renderer::init_draw_image() -> bool {
		m_DrawImage.extent = vk::Extent3D(m_Width, m_Height, 1);
		m_DrawImage.format = vk::Format::eR16G16B16A16Sfloat;

		vk::ImageCreateInfo draw_image_ci(
		    vk::ImageCreateFlags(0),
		    vk::ImageType::e2D,
		    m_DrawImage.format,
		    m_DrawImage.extent,
		    1, /* mip levels */
		    1, /* array layers*/
		    vk::SampleCountFlagBits::e1,
		    vk::ImageTiling::eOptimal,
		    vk::ImageUsageFlagBits::eTransferSrc |
		        vk::ImageUsageFlagBits::eTransferDst |
		        vk::ImageUsageFlagBits::eStorage |
		        vk::ImageUsageFlagBits::eColorAttachment);

		VmaAllocationCreateInfo draw_image_allocinfo = {};
		draw_image_allocinfo.usage                   = VMA_MEMORY_USAGE_GPU_ONLY;
		draw_image_allocinfo.requiredFlags           = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		vkcheck(vmaCreateImage(
		            m_VMA,
		            vkcast(draw_image_ci),
		            &draw_image_allocinfo,
		            vkcast(m_DrawImage.img),
		            &m_DrawImage.alloc,
		            nullptr),
		        "failed to create draw image image (using VMA)");

		vk::ImageViewCreateInfo draw_image_view_ci(
		    vk::ImageViewCreateFlags(0),
		    m_DrawImage.img,
		    vk::ImageViewType::e2D,
		    m_DrawImage.format,
		    vk::ComponentMapping{},
		    vk::ImageSubresourceRange(
		        vk::ImageAspectFlagBits::eColor,
		        0, /* base mip level */
		        1, /* level count */
		        0, /* base array layer */
		        1  /* layer count */
		        ));

		vkcheck(vkCreateImageView(
		            m_Device.device,
		            vkcast(draw_image_view_ci),
		            allocator(),
		            vkcast(m_DrawImage.view)),
		        "failed to create draw image view");

		return true;
	}

	auto Renderer::init_descriptors() -> bool {
		{
			std::vector<PoolSizeRatio> pool_size_ratios{
				PoolSizeRatio{ vk::DescriptorType::eUniformBuffer, 1 },
			};
			if (!m_DescAllocator.init(16, pool_size_ratios, vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind)) {
				return false;
			}
		}

		{
			std::vector<PoolSizeRatio> pool_size_ratios{
				PoolSizeRatio{ vk::DescriptorType::eCombinedImageSampler, 16384 }
			};

			if (!m_TexAllocator.init(1, pool_size_ratios, vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind)) {
				return false;
			}

			vk::DescriptorSetLayoutBinding binding(
			    0,
			    vk::DescriptorType::eCombinedImageSampler,
			    16384,
			    vk::ShaderStageFlagBits::eAll);

			vk::DescriptorBindingFlags flags = vk::DescriptorBindingFlagBits::ePartiallyBound |
			                                   vk::DescriptorBindingFlagBits::eUpdateAfterBind;

			vk::DescriptorSetLayoutBindingFlagsCreateInfo flags_create_info(
			    1,
			    &flags);

			vk::DescriptorSetLayoutCreateInfo texture_descriptor_set_create_info(
			    vk::DescriptorSetLayoutCreateFlagBits::eUpdateAfterBindPool,
			    1,
			    &binding,
			    &flags_create_info);

			vkcheck(vkCreateDescriptorSetLayout(
			            m_Device.device,
			            vkcast(texture_descriptor_set_create_info),
			            allocator(),
			            vkcast(m_TextureDescriptorLayout)),
			        "failed to create texture descriptor set layout");

			m_TextureDescriptors = m_TexAllocator.alloc(m_TextureDescriptorLayout);

			m_GC.push([&]() {
				vkDestroyDescriptorSetLayout(m_Device.device, m_TextureDescriptorLayout, allocator());
			});
		}

		return true;
	}

	auto Renderer::deinit() -> void {
		while (vkDeviceWaitIdle(m_Device.device) != VK_SUCCESS)
			;

		auto& immediate_cmds = get_immediate();
		immediate_cmds.destroy();

		for (auto& render_pass : m_RenderPasses) {
			render_pass->destroy();
		}

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroyCommandPool(m_Device.device, m_Frames[i].pool, allocator());
			vkDestroyFence(m_Device, m_Frames[i].render_fence, allocator());
			vkDestroySemaphore(m_Device, m_Frames[i].acquire, allocator());
			m_Frames[i].pool         = VK_NULL_HANDLE;
			m_Frames[i].render_fence = VK_NULL_HANDLE;
			m_Frames[i].acquire      = VK_NULL_HANDLE;
		}

		vkDestroyImageView(m_Device.device, m_DrawImage.view, allocator());
		vmaDestroyImage(m_VMA, m_DrawImage.img, m_DrawImage.alloc);

		m_DrawImage.view  = VK_NULL_HANDLE;
		m_DrawImage.img   = VK_NULL_HANDLE;
		m_DrawImage.alloc = VK_NULL_HANDLE;

		vkb::destroy_swapchain(m_Swapchain);
		m_Swapchain.swapchain = VK_NULL_HANDLE;

		for (size_t i = 0; i < m_Images.size(); i++) {
			vkDestroyImageView(m_Device.device, m_Images[i].view, allocator());
			vkDestroySemaphore(m_Device, m_Images[i].render_finished, allocator());
			m_Images[i].view            = VK_NULL_HANDLE;
			m_Images[i].render_finished = VK_NULL_HANDLE;
		}

		m_GC.run();

		m_DescAllocator.deinit();
		m_TexAllocator.deinit();

		vmaDestroyAllocator(m_VMA);
		m_VMA = VK_NULL_HANDLE;

		vkb::destroy_surface(m_Instance, m_Surface);
		vkb::destroy_device(m_Device);
		vkb::destroy_instance(m_Instance);
		m_Surface                        = VK_NULL_HANDLE;
		m_Device.device                  = VK_NULL_HANDLE;
		m_Device.physical_device.surface = VK_NULL_HANDLE;
		m_Instance.instance              = VK_NULL_HANDLE;
	}

	auto Renderer::on_begin() -> bool {
		auto& frame = get_current_frame();

		vkcheck(vkWaitForFences(
		            m_Device.device,
		            1,
		            vkcast(frame.render_fence),
		            true,
		            1000000000),
		        "failed to wait for render fence");

		vkcheck(vkResetFences(
		            m_Device.device,
		            1,
		            vkcast(frame.render_fence)),
		        "failed to reset render fence");

		vkcheck(vkAcquireNextImageKHR(
		            m_Device.device,
		            m_Swapchain,
		            1000000000,
		            frame.acquire,
		            nullptr,
		            &m_SwapchainImgIndex),
		        "failed to acquire next swapchain image");

		vkcheck(vkResetCommandBuffer(frame.cmd, 0), "failed to reset command buffer");

		vk::CommandBufferBeginInfo cbbi(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
		vkcheck(vkBeginCommandBuffer(
		            frame.cmd,
		            vkcast(cbbi)),
		        "failed to begin command buffer");

		transition_image(
		    frame.cmd,
		    m_DrawImage.img,
		    vk::ImageLayout::eUndefined,
		    vk::ImageLayout::eGeneral);

		vk::ImageSubresourceRange clear_range(
		    vk::ImageAspectFlagBits::eColor,
		    0,
		    VK_REMAINING_MIP_LEVELS,
		    0,
		    VK_REMAINING_ARRAY_LAYERS);

		clear_screen(frame.cmd, m_DrawImage.img, vk::ImageLayout::eGeneral, m_ClearColor, clear_range);

		auto swapchain_img = m_Images[m_SwapchainImgIndex].img;

		transition_image(
		    frame.cmd,
		    m_DrawImage.img,
		    vk::ImageLayout::eGeneral,
		    vk::ImageLayout::eColorAttachmentOptimal);

		vk::RenderingAttachmentInfo color_attachment(
		    m_DrawImage.view,
		    vk::ImageLayout::eColorAttachmentOptimal,
		    vk::ResolveModeFlagBits::eNone,
		    {}, /* resolve image view */
		    vk::ImageLayout::eUndefined,
		    vk::AttachmentLoadOp::eLoad,
		    vk::AttachmentStoreOp::eStore);

		vk::RenderingInfo render_info(
		    vk::RenderingFlags{},
		    vk::Rect2D(
		        vk::Offset2D{},
		        vk::Extent2D(m_DrawImage.extent.width, m_DrawImage.extent.height)),
		    1, /* layer count*/
		    0, /* view mask */
		    1, /* color attachment count */
		    &color_attachment);

		vkCmdBeginRendering(frame.cmd, vkcast(render_info));
		return true;
	}

	auto Renderer::on_end() -> bool {
		auto& frame = get_current_frame();
		auto& img   = m_Images[m_SwapchainImgIndex];

		for (auto& render_pass : m_RenderPasses) {
			render_pass->set_bind_point(vk::PipelineBindPoint::eGraphics);
			render_pass->set_cmd_buffer(frame.cmd);
			render_pass->bind();
			render_pass->set_viewport({ static_cast<float>(m_DrawImage.extent.width),
			                            static_cast<float>(m_DrawImage.extent.height) });
			render_pass->set_scissor({ 0.f, 0.f }, { static_cast<float>(m_DrawImage.extent.width),
			                                         static_cast<float>(m_DrawImage.extent.height) });
			render_pass->run();
			render_pass->clear();
		}

		vkCmdEndRendering(frame.cmd);

		transition_image(
		    frame.cmd,
		    m_DrawImage.img,
		    vk::ImageLayout::eColorAttachmentOptimal,
		    vk::ImageLayout::eTransferSrcOptimal);

		transition_image(
		    frame.cmd,
		    img.img,
		    vk::ImageLayout::eUndefined,
		    vk::ImageLayout::eTransferDstOptimal);

		copy_image_to_image(
		    frame.cmd,
		    m_DrawImage.img,
		    vk::Extent2D(m_DrawImage.extent.width, m_DrawImage.extent.height),
		    img.img,
		    m_Swapchain.extent);

		transition_image(
		    frame.cmd,
		    img.img,
		    vk::ImageLayout::eTransferDstOptimal,
		    vk::ImageLayout::ePresentSrcKHR);

		vkcheck(vkEndCommandBuffer(frame.cmd), "failed to end command buffer");

		vk::CommandBufferSubmitInfo cbsi(frame.cmd);

		vk::SemaphoreSubmitInfo wait_ssi(
		    frame.acquire,
		    1,
		    vk::PipelineStageFlagBits2::eColorAttachmentOutputKHR);

		vk::SemaphoreSubmitInfo signal_ssi(
		    img.render_finished,
		    1,
		    vk::PipelineStageFlagBits2::eAllGraphics);

		vk::SubmitInfo2 submit_info(
		    {},
		    1,
		    &wait_ssi,
		    1,
		    &cbsi,
		    1,
		    &signal_ssi);

		vkQueueSubmit2(m_GraphicsQueue, 1, vkcast(submit_info), frame.render_fence);
		vk::PresentInfoKHR present_info(
		    1,
		    &img.render_finished,
		    1,
		    reinterpret_cast<vk::SwapchainKHR*>(&m_Swapchain.swapchain),
		    &m_SwapchainImgIndex);

		auto result = vkQueuePresentKHR(m_GraphicsQueue, vkcast(present_info));

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
			if (m_Width == 0 || m_Height == 0 || !m_Swapchain)
				return false;

			aby_rhi_dbg("recreating swapchain: [w: {}, h: {}, result: {}]",
			            m_Width, m_Height, string_VkResult(result));

			recreate_swapchain();
			return false;
		}

		m_FrameIndex++;

		return true;
	}

	auto Renderer::on_resize(uint32_t width, uint32_t height) -> void {
		m_Width  = width;
		m_Height = height;
	}

	auto Renderer::recreate_swapchain() -> bool {
		while (vkDeviceWaitIdle(m_Device.device) != VK_SUCCESS)
			;

		vkb::SwapchainBuilder swapchain_builder(m_Device.physical_device, m_Device.device, m_Surface, m_GraphicsQueueFamily, m_PresentQueueFamily);
		auto swapchain_result = swapchain_builder
		                            .set_desired_extent(m_Width, m_Height)
		                            .set_desired_format(VkSurfaceFormatKHR{ .format = VK_FORMAT_R8G8B8A8_UNORM })
		                            .set_allocation_callbacks(allocator())
		                            .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
		                            .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
		                            .add_image_usage_flags(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
		                            .set_desired_min_image_count(MAX_FRAMES_IN_FLIGHT)
		                            .set_old_swapchain(m_Swapchain)
		                            .build();

		vkbcheck(swapchain_result, "failed to recreate swapchain");

		if (m_Swapchain) {
			vkb::destroy_swapchain(m_Swapchain);
		}

		m_Swapchain        = swapchain_result.value();
		auto [imgs, views] = m_Swapchain.get_images_and_image_views().value();
		for (size_t i = 0; i < m_Images.size(); i++) {
			if (m_Images[i].view) {
				vkDestroyImageView(m_Device.device, m_Images[i].view, allocator());
				m_Images[i].view = VK_NULL_HANDLE;
			}
		}

		if (m_Images.size() < imgs.size()) {
			m_Images.resize(imgs.size());
		}

		for (size_t i = 0; i < imgs.size(); i++) {
			m_Images[i].img  = imgs[i];
			m_Images[i].view = views[i];
			if (!m_Images[i].render_finished) {
				vk::SemaphoreCreateInfo semaphore_ci;
				vkcheck(vkCreateSemaphore(
				            m_Device.device,
				            vkcast(semaphore_ci),
				            allocator(),
				            vkcast(m_Images[i].render_finished)),
				        "failed to create wait semaphore");
			}
		}

		return true;
	}

	auto Renderer::get_extensions(std::vector<const char*>* inst_exts, std::vector<const char*>* dev_exts) -> bool {
		auto& ctx = Context::get();
		if (!get_instance_extensions(inst_exts)) {
			return false;
		}

		dev_exts->reserve(5);
		dev_exts->push_back(VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME);
		dev_exts->push_back(VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME);
		dev_exts->push_back(VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME);
		dev_exts->push_back(VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME);
		dev_exts->push_back(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME);

		return true;
	}

	auto Renderer::get_current_frame() -> FrameData& {
		return m_Frames[m_FrameIndex % MAX_FRAMES_IN_FLIGHT];
	}

	auto Renderer::get_immediate() -> ImmediateCommands& {
		thread_local ImmediateCommands cmds = create_immediate_commands();
		return cmds;
	}

	auto Renderer::create_immediate_commands() -> ImmediateCommands {
		vk::CommandPoolCreateInfo command_pool_ci(
		    vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
		    m_GraphicsQueueFamily);

		ImmediateCommands cmds;

		vkassert(vkCreateCommandPool(
		             m_Device.device,
		             vkcast(command_pool_ci),
		             allocator(),
		             vkcast(cmds.pool)),
		         "failed to create immediate submit command pool");

		vk::CommandBufferAllocateInfo cmd_alloc_info(cmds.pool, vk::CommandBufferLevel::ePrimary, 1);

		vkassert(vkAllocateCommandBuffers(
		             m_Device.device,
		             vkcast(cmd_alloc_info),
		             vkcast(cmds.cmd)),
		         "failed to allocate immediate command buffer");

		vk::FenceCreateInfo fence_ci(vk::FenceCreateFlagBits::eSignaled);

		vkassert(vkCreateFence(
		             m_Device.device,
		             vkcast(fence_ci),
		             allocator(),
		             vkcast(cmds.fence)),
		         "failed to create render fence");

		return cmds;
	}

	

	auto Renderer::immediate_submit(std::function<void(vk::CommandBuffer)>&& fn) -> bool {
		std::scoped_lock lock(m_ImmediateSubmitMutex);
		
		auto& immediate = get_immediate();

		vkcheck(vkResetFences(m_Device.device, 1, vkcast(immediate.fence)), "failed to reset immediate submit fence");
		vkcheck(vkResetCommandBuffer(immediate.cmd, 0), "failed to reset immedaite submit command buffer");

		vk::CommandBufferBeginInfo begin_info(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
		vkcheck(vkBeginCommandBuffer(immediate.cmd, vkcast(begin_info)), "failed to begin immediate submit command buffer");

		fn(immediate.cmd);

		vkcheck(vkEndCommandBuffer(immediate.cmd),
		        "failed to end immediate submit command buffer");

		vk::CommandBufferSubmitInfo submit_info(immediate.cmd);
		vk::SubmitInfo2 submit(vk::SubmitFlags(), 0, nullptr, 1, &submit_info);

		vkcheck(vkQueueSubmit2(
		            m_GraphicsQueue,
		            1, /* submit count */
		            vkcast(submit),
		            immediate.fence),
		        "failed to submit immediate submit command buffer");

		vkcheck(vkWaitForFences(
		            m_Device.device,
		            1, /* fence count */
		            vkcast(immediate.fence),
		            vk::True,
		            9999999999 /* timeout */
		            ),
		        "failed to wait for immediate submit fence");

		return true;
	}

	auto Renderer::register_texture(vk::ImageView view, vk::Sampler sampler) -> uint32_t {
		static uint32_t global_texture_index = 0;

		uint32_t texture_id = global_texture_index;

		vk::DescriptorImageInfo info(sampler, view, vk::ImageLayout::eShaderReadOnlyOptimal);
		vk::WriteDescriptorSet write(m_TextureDescriptors, 0, texture_id, 1, vk::DescriptorType::eCombinedImageSampler, &info);

		vkUpdateDescriptorSets(m_Device.device, 1, vkcast(write), 0, nullptr);

		global_texture_index++;
		return texture_id;
	}

	auto Renderer::set_clear_color(Color color) -> void {
		switch (m_Swapchain.image_format) {
			case VK_FORMAT_R8_UINT:
			case VK_FORMAT_R8G8_UINT:
			case VK_FORMAT_R8G8B8_UINT:
			case VK_FORMAT_R8G8B8A8_UINT:
				m_ClearColor.uint32[0] = uint32_t(color.r * UINT8_MAX + 0.5f);
				m_ClearColor.uint32[1] = uint32_t(color.g * UINT8_MAX + 0.5f);
				m_ClearColor.uint32[2] = uint32_t(color.b * UINT8_MAX + 0.5f);
				m_ClearColor.uint32[3] = uint32_t(color.a * UINT8_MAX + 0.5f);
				break;
			case VK_FORMAT_R16_UINT:
			case VK_FORMAT_R16G16_UINT:
			case VK_FORMAT_R16G16B16_UINT:
			case VK_FORMAT_R16G16B16A16_UINT:
				m_ClearColor.uint32[0] = uint32_t(color.r * UINT16_MAX + 0.5f);
				m_ClearColor.uint32[1] = uint32_t(color.g * UINT16_MAX + 0.5f);
				m_ClearColor.uint32[2] = uint32_t(color.b * UINT16_MAX + 0.5f);
				m_ClearColor.uint32[3] = uint32_t(color.a * UINT16_MAX + 0.5f);
				break;
			case VK_FORMAT_R32_UINT:
			case VK_FORMAT_R32G32_UINT:
			case VK_FORMAT_R32G32B32_UINT:
			case VK_FORMAT_R32G32B32A32_UINT:
			case VK_FORMAT_R64_UINT:
			case VK_FORMAT_R64G64_UINT:
			case VK_FORMAT_R64G64B64_UINT:
			case VK_FORMAT_R64G64B64A64_UINT:
				m_ClearColor.uint32[0] = uint32_t(color.r * float(UINT32_MAX));
				m_ClearColor.uint32[1] = uint32_t(color.g * float(UINT32_MAX));
				m_ClearColor.uint32[2] = uint32_t(color.b * float(UINT32_MAX));
				m_ClearColor.uint32[3] = uint32_t(color.a * float(UINT32_MAX));
				break;
			case VK_FORMAT_R8_SINT:
			case VK_FORMAT_R8G8_SINT:
			case VK_FORMAT_R8G8B8_SINT:
			case VK_FORMAT_R8G8B8A8_SINT:
				m_ClearColor.int32[0] = int32_t(std::round(color.r * INT8_MAX));
				m_ClearColor.int32[1] = int32_t(std::round(color.g * INT8_MAX));
				m_ClearColor.int32[2] = int32_t(std::round(color.b * INT8_MAX));
				m_ClearColor.int32[3] = int32_t(std::round(color.a * INT8_MAX));
				break;
			case VK_FORMAT_R16_SINT:
			case VK_FORMAT_R16G16_SINT:
			case VK_FORMAT_R16G16B16_SINT:
			case VK_FORMAT_R16G16B16A16_SINT:
				m_ClearColor.int32[0] = int32_t(std::round(color.r * INT16_MAX));
				m_ClearColor.int32[1] = int32_t(std::round(color.g * INT16_MAX));
				m_ClearColor.int32[2] = int32_t(std::round(color.b * INT16_MAX));
				m_ClearColor.int32[3] = int32_t(std::round(color.a * INT16_MAX));
				break;
			case VK_FORMAT_R32_SINT:
			case VK_FORMAT_R32G32_SINT:
			case VK_FORMAT_R32G32B32_SINT:
			case VK_FORMAT_R32G32B32A32_SINT:
			case VK_FORMAT_R64_SINT:
			case VK_FORMAT_R64G64_SINT:
			case VK_FORMAT_R64G64B64_SINT:
			case VK_FORMAT_R64G64B64A64_SINT:
				m_ClearColor.int32[0] = int32_t(std::round(color.r * INT32_MAX));
				m_ClearColor.int32[1] = int32_t(std::round(color.g * INT32_MAX));
				m_ClearColor.int32[2] = int32_t(std::round(color.b * INT32_MAX));
				m_ClearColor.int32[3] = int32_t(std::round(color.a * INT32_MAX));
				// normalzie from -128 -> +128
				break;
			default:
				m_ClearColor.float32[0] = color.r;
				m_ClearColor.float32[1] = color.g;
				m_ClearColor.float32[2] = color.b;
				m_ClearColor.float32[3] = color.a;
				break;
		}
	}

	auto Renderer::device() -> vkb::Device& {
		return m_Device;
	}

	auto Renderer::add_pass(std::shared_ptr<rhi::RenderPass> render_pass) -> void {
		m_RenderPasses.push_back(std::static_pointer_cast<RenderPass>(render_pass));
	}

	auto Renderer::vma() -> VmaAllocator& {
		return m_VMA;
	}

	auto Renderer::color_format() -> vk::Format {
		return m_DrawImage.format;
	}

	auto Renderer::gc() -> GarbageCollector& {
		return m_GC;
	}

	auto Renderer::desc_alloc() -> DescriptorAllocator& {
		return m_DescAllocator;
	}

	auto Renderer::tex_desc_set() -> vk::DescriptorSet {
		return m_TextureDescriptors;
	}

	auto Renderer::tex_desc_layout() -> vk::DescriptorSetLayout {
		return m_TextureDescriptorLayout;
	}

} // namespace aby::rhi::vulkan

namespace aby::rhi::vulkan {

	ImmediateCommands::~ImmediateCommands() {
		destroy();
	}

	auto ImmediateCommands::destroy() -> void {
		if (this->pool) {
			auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
			vkDestroyCommandPool(r->device(), this->pool, allocator());
			vkDestroyFence(r->device(), this->fence, allocator());
			this->pool  = VK_NULL_HANDLE;
			this->fence = VK_NULL_HANDLE;
		}
	}

} // namespace aby::rhi::vulkan
