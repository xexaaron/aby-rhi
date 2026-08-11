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

	Renderer::Renderer(GraphicsParams params) :
	    m_Graphics(params) {
	}

	auto Renderer::on_begin() -> bool {
		if (!m_Frames->begin(m_Swapchain.swapchain, &m_SwapchainImgIndex)) {
			return false;
		}

		auto& [swapchain_img, render_finished_semaphore] = m_SwapchainImages[m_SwapchainImgIndex];

		m_DrawImage.transition(m_Frames->cmd(), vk::ImageLayout::eColorAttachmentOptimal);

		if (antialiasing_enabled()) {
			m_ResolveImage.transition(m_Frames->cmd(), vk::ImageLayout::eColorAttachmentOptimal);
		}

		vk::RenderingAttachmentInfo color_attachment(
		    m_DrawImage.view(),
		    vk::ImageLayout::eColorAttachmentOptimal,
		    antialiasing_enabled()
		        ? vk::ResolveModeFlagBits::eAverage
		        : vk::ResolveModeFlagBits::eNone,
		    m_ResolveImage.view(),
		    antialiasing_enabled()
		        ? vk::ImageLayout::eColorAttachmentOptimal
		        : vk::ImageLayout::eUndefined,
		    vk::AttachmentLoadOp::eClear,
		    vk::AttachmentStoreOp::eStore,
		    m_ClearColor);

		vk::RenderingInfo render_info(
		    vk::RenderingFlags{},
		    vk::Rect2D(
		        vk::Offset2D{},
		        m_DrawImage.extent2d()),
		    1, /* layer count*/
		    0, /* view mask */
		    1, /* color attachment count */
		    &color_attachment);

		vkCmdBeginRendering(m_Frames->cmd(), vkcast(render_info));
		return true;
	}

	auto Renderer::on_end() -> bool {
		auto& [swapchain_img, render_finished_semaphore] = m_SwapchainImages[m_SwapchainImgIndex];

		for (auto& render_pass : m_RenderPasses) {
			render_pass->set_bind_point(vk::PipelineBindPoint::eGraphics);
			render_pass->set_cmd_buffer(m_Frames->cmd());
			render_pass->bind();
			render_pass->set_viewport({ static_cast<float>(m_DrawImage.width()),
			                            static_cast<float>(m_DrawImage.height()) });
			render_pass->set_scissor({ 0.f, 0.f }, { static_cast<float>(m_DrawImage.width()),
			                                         static_cast<float>(m_DrawImage.height()) });
			render_pass->run();
			render_pass->clear();
		}

		vkCmdEndRendering(m_Frames->cmd());

		Image& present_image = antialiasing_enabled() ? m_ResolveImage : m_DrawImage;

		present_image.transition(m_Frames->cmd(), vk::ImageLayout::eTransferSrcOptimal);
		swapchain_img.transition(m_Frames->cmd(), vk::ImageLayout::eTransferDstOptimal);
		present_image.copy_to(m_Frames->cmd(), swapchain_img);
		swapchain_img.transition(m_Frames->cmd(), vk::ImageLayout::ePresentSrcKHR);

		vk::Result result = m_Frames->end(m_Swapchain.swapchain, m_GraphicsQueue, render_finished_semaphore, m_SwapchainImgIndex);

		if (result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) {
			if (m_Width == 0 || m_Height == 0 || !m_Swapchain)
				return false;

			aby_rhi_dbg("recreating swapchain: [w: {}, h: {}, result: {}]",
			            m_Width, m_Height, vk::to_string(result));

			recreate_swapchain();
			return false;
		}

		m_Frames++;

		return true;
	}

	auto Renderer::on_resize(uint32_t width, uint32_t height) -> void {
		m_Width  = width;
		m_Height = height;
	}

	auto Renderer::recreate_swapchain() -> bool {
		aby_rhi_profile("[vulkan] recreate swapchain");

		if (m_Swapchain) { // fast init first time
			while (vkDeviceWaitIdle(m_Device.device) != VK_SUCCESS)
				;
		}

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
		for (auto& [img, _] : m_SwapchainImages) {
			img.destroy();
		}

		if (m_SwapchainImages.size() < imgs.size()) {
			m_SwapchainImages.resize(imgs.size());
		}

		for (size_t i = 0; i < imgs.size(); i++) {
			m_SwapchainImages[i].first.wrap(
			    imgs[i],
			    views[i],
			    vk::Extent3D(m_Swapchain.extent.width, m_Swapchain.extent.height, 1),
			    static_cast<vk::Format>(m_Swapchain.image_format),
			    vk::SampleCountFlagBits::e1,
			    static_cast<vk::ImageUsageFlags>(m_Swapchain.image_usage_flags));

			if (!m_SwapchainImages[i].second) {
				vk::SemaphoreCreateInfo semaphore_ci;
				vkcheck(vkCreateSemaphore(
				            m_Device.device,
				            vkcast(semaphore_ci),
				            allocator(),
				            vkcast(m_SwapchainImages[i].second)),
				        "failed to create wait semaphore");
			}
		}

		return true;
	}

	auto Renderer::get_immediate() -> ImmediateCommands& {
		thread_local ImmediateCommands cmds;
		cmds.create(m_GraphicsQueueFamily);
		return cmds;
	}

	auto Renderer::immediate_submit(std::function<void(vk::CommandBuffer)>&& fn) -> bool {
		std::scoped_lock lock(m_ImmediateSubmitMutex);
		auto& immediate = get_immediate();

		if (!immediate.begin()) {
			aby_rhi_err("failed to begin immediate command submission");
			return false;
		}

		fn(immediate.cmd());

		if (!immediate.end(m_GraphicsQueue)) {
			aby_rhi_err("failed to end immediate command submission");
			return false;
		}

		return true;
	}

	auto Renderer::register_texture(ResourceID id, vk::ImageView view, vk::Sampler sampler) -> uint32_t {
		uint32_t texture_id = static_cast<uint32_t>(id);
		vk::DescriptorImageInfo info(sampler, view, vk::ImageLayout::eShaderReadOnlyOptimal);
		vk::WriteDescriptorSet write(m_TextureDescriptors, 0, texture_id, 1, vk::DescriptorType::eCombinedImageSampler, &info);
		vkUpdateDescriptorSets(m_Device.device, 1, vkcast(write), 0, nullptr);
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
		return m_DrawImage.format();
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

	auto Renderer::max_sampler_anisotropy() -> float {
		return m_Limits.maxSamplerAnisotropy;
	}

	auto Renderer::graphics() const -> const GraphicsParams& {
		return m_Graphics;
	}

	auto Renderer::render_target_sample_count() -> vk::SampleCountFlagBits {
		switch (m_Graphics.aliasing) {
			case EAntiAliasing::none:
				return vk::SampleCountFlagBits::e1;
			case EAntiAliasing::msaa2x:
				return vk::SampleCountFlagBits::e2;
			case EAntiAliasing::msaa4x:
				return vk::SampleCountFlagBits::e4;
			case EAntiAliasing::msaa8x:
				return vk::SampleCountFlagBits::e8;
		}
		return vk::SampleCountFlagBits::e1;
	}

	auto Renderer::antialiasing_enabled() const -> bool {
		return m_Graphics.aliasing != EAntiAliasing::none;
	}

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

		if (!m_Frames.create(m_GraphicsQueueFamily)) {
			aby_rhi_err("failed to create commands");
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
		aby_rhi_profile("[vulkan] init vulkan");

		auto& ctx = Context::get();
		auto* log = ctx.logger();

		std::vector<const char*> instance_extensions, device_extensions;
		if (!get_extensions(&instance_extensions, &device_extensions)) {
			return false;
		}

		vkb::InstanceBuilder instb;
		auto inst_ret = instb.set_app_name("aby-rhi")
		                    .set_allocation_callbacks(allocator())
		                    .set_debug_callback(vk_debug_callback)
		                    .set_debug_messenger_severity(VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
		                                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT |
#if SHADER_PRINTF_ENABLE == 1
		                                                  VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT
#endif
		                                                  )
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
		features.setSamplerAnisotropy(vk::True)
#if SHADER_PRINTF_ENABLE == 1
		    .setFragmentStoresAndAtomics(vk::True)
		    .setVertexPipelineStoresAndAtomics(vk::True)
		    .setShaderInt64(vk::True)
		    .setShaderInt16(vk::True);
#else
		    ;
#endif

		vkb::PhysicalDeviceSelector selector(inst_ret.value());
		auto gpu_ret = selector
		                   .set_minimum_version(1, 4)
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
		m_Limits = props.limits;
		return true;
	}

	auto Renderer::init_vma() -> bool {
		aby_rhi_profile("[vulkan] init VMA");
		VmaAllocatorCreateInfo alloc_info = {};
		alloc_info.physicalDevice         = m_Device.physical_device;
		alloc_info.device                 = m_Device.device;
		alloc_info.instance               = m_Instance;
		alloc_info.flags                  = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
		vkcheck(vmaCreateAllocator(&alloc_info, &m_VMA), "failed to create VMA allocator");

		return true;
	}

	auto Renderer::init_draw_image() -> bool {
		aby_rhi_profile("[vulkan] init draw image");
		auto sample_count = render_target_sample_count();
		auto extent       = vk::Extent3D(m_Width, m_Height, 1);
		auto format       = vk::Format::eR16G16B16A16Sfloat;
		auto usage        = vk::ImageUsageFlagBits::eTransferSrc |
		                    vk::ImageUsageFlagBits::eTransferDst |
		                    vk::ImageUsageFlagBits::eColorAttachment;

		m_DrawImage.create(extent, format, sample_count, usage);

		if (antialiasing_enabled()) {
			m_ResolveImage.create(extent, format, vk::SampleCountFlagBits::e1, usage);
		}

		return true;
	}

	auto Renderer::init_descriptors() -> bool {
		aby_rhi_profile("[vulkan] init descriptors");
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
		aby_rhi_profile("[vulkan] deinit");

		while (vkDeviceWaitIdle(m_Device.device) != VK_SUCCESS)
			;

		auto& immediate_cmds = get_immediate();
		immediate_cmds.destroy();

		for (auto& render_pass : m_RenderPasses) {
			render_pass->destroy();
		}

		m_Frames.destroy();

		m_DrawImage.destroy();
		m_ResolveImage.destroy();

		vkb::destroy_swapchain(m_Swapchain);
		m_Swapchain.swapchain = VK_NULL_HANDLE;

		for (size_t i = 0; i < m_SwapchainImages.size(); i++) {
			auto& [img, semaphore] = m_SwapchainImages[i];
			img.destroy();
			vkDestroySemaphore(m_Device, semaphore, allocator());
			semaphore = VK_NULL_HANDLE;
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

} // namespace aby::rhi::vulkan
