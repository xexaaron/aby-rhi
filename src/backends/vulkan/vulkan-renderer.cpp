#include "backends/vulkan/vulkan-renderer.hpp"
#include "backends/vulkan/vulkan-callbacks.hpp"
#include "backends/vulkan/vulkan-platform.hpp"
#include "backends/vulkan/vulkan-helpers.hpp"
#include "backends/vulkan/vulkan-pipeline.hpp"
#include "context.hpp"
#include <vector>
#include <VkBootstrap.h>

namespace aby::rhi::vulkan {

    auto Renderer::init(void* native_window) -> bool {
        auto& ctx = Context::get();
        auto* log = ctx.logger();
        std::vector<const char*> instance_extensions, device_extensions;
        if (!get_extensions(&instance_extensions, &device_extensions)) {
            return false;
        }

#ifndef NDEBUG
        aby_rhi_dbg("[vulkan] instance extensions: {}", instance_extensions.size());
        for (size_t i = 0; i < instance_extensions.size(); i++) {
            aby_rhi_dbg("[vulkan] -- {}) {}", i, instance_extensions[i]);
        }
#endif
     
        vkb::InstanceBuilder instb;
        auto inst_ret = instb.set_app_name("aby-rhi")
            .set_allocation_callbacks(allocator())
            .set_debug_callback(vk_debug_callback)
#ifndef NDEBUG
            .request_validation_layers(true)
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
        vk::PhysicalDeviceVulkan12Features features12;
        features12.setBufferDeviceAddress(vk::True)
                  .setDescriptorIndexing(vk::True)
                  .setTimelineSemaphore(vk::True);
        vk::PhysicalDeviceVulkan13Features features13;
        features13.setDynamicRendering(vk::True)
                  .setSynchronization2(vk::True);
        vk::PhysicalDeviceVulkan14Features features14;

        vkb::PhysicalDeviceSelector selector(inst_ret.value());
        auto gpu_ret = selector
            .set_minimum_version(1, 3)
            .set_required_features_14(features14)
            .set_required_features_13(features13)
            .set_required_features_12(features12)
            .set_required_features_11(features11)
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

#ifndef NDEBUG
        VkPhysicalDeviceProperties props = {0};
        vkGetPhysicalDeviceProperties(m_Device.physical_device, &props);
        aby_rhi_dbg("[vulkan] found GPU: {}", props.deviceName);
#endif

        vkb::SwapchainBuilder swapchain_builder(m_Device.physical_device, m_Device.device, m_Surface, m_GraphicsQueueFamily, m_PresentQueueFamily);
        auto swapchain_result = swapchain_builder.set_desired_extent(m_Width, m_Height)
            .set_desired_format(VkSurfaceFormatKHR{.format = VK_FORMAT_R8G8B8A8_UNORM })
            .set_allocation_callbacks(allocator())
            .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
            .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .add_image_usage_flags(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
            .set_desired_min_image_count(MAX_FRAMES_IN_FLIGHT)
            .build();

        vkbcheck(swapchain_result, "failed to create swapchain");

        m_Swapchain = swapchain_result.value();
        auto [imgs, views] = m_Swapchain.get_images_and_image_views().value();
        m_Images.resize(imgs.size());
        for (size_t i = 0; i < imgs.size(); i++) {
            m_Images[i].img  = imgs[i];
            m_Images[i].view = views[i];
            vk::SemaphoreCreateInfo semaphore_ci;
            vkcheck(vkCreateSemaphore(
                m_Device.device,
                reinterpret_cast<VkSemaphoreCreateInfo*>(&semaphore_ci),
                allocator(),
                reinterpret_cast<VkSemaphore*>(&m_Images[i].render_finished)
            ), "failed to create acquire semaphore");
        }
        
        vk::CommandPoolCreateInfo command_pool_ci(
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            m_GraphicsQueueFamily
        );
        
        for (size_t i = 0; i < m_Frames.size(); i++) {
            vkcheck(vkCreateCommandPool(
                m_Device.device,
                reinterpret_cast<const VkCommandPoolCreateInfo*>(&command_pool_ci),
                allocator(), 
                reinterpret_cast<VkCommandPool*>(&m_Frames[i].pool)
            ), "failed to create command pool");
            
            vk::CommandBufferAllocateInfo cmd_alloc_info(m_Frames[i].pool, vk::CommandBufferLevel::ePrimary, 1);

            vkcheck(vkAllocateCommandBuffers(
                m_Device.device, 
                reinterpret_cast<const VkCommandBufferAllocateInfo*>(&cmd_alloc_info),
                reinterpret_cast<VkCommandBuffer*>(&m_Frames[i].cmd)
            ), "failed to create command pool");

            vk::FenceCreateInfo fence_ci(vk::FenceCreateFlagBits::eSignaled);
            vkcheck(vkCreateFence(
                m_Device.device,
                reinterpret_cast<VkFenceCreateInfo*>(&fence_ci),
                allocator(),
                reinterpret_cast<VkFence*>(&m_Frames[i].render_fence)
            ), "failed to create render fence");

            vk::SemaphoreCreateInfo semaphore_ci;
            vkcheck(vkCreateSemaphore(
                m_Device.device,
                reinterpret_cast<VkSemaphoreCreateInfo*>(&semaphore_ci),
                allocator(),
                reinterpret_cast<VkSemaphore*>(&m_Frames[i].acquire)
            ), "failed to create wait semaphore");
        }

        m_ClearColor = vk::ClearColorValue(0.f, 0.f, 0.f, 1.f);

        VmaAllocatorCreateInfo alloc_info = {};
        alloc_info.physicalDevice = m_Device.physical_device;
        alloc_info.device         = m_Device.device;
        alloc_info.instance       = m_Instance;
        alloc_info.flags          = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
        vkcheck(vmaCreateAllocator(&alloc_info, &m_VMA), "failed to create VMA allocator");

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
            vk::ImageUsageFlagBits::eTransferSrc     |
            vk::ImageUsageFlagBits::eTransferDst     |
            vk::ImageUsageFlagBits::eStorage         |
            vk::ImageUsageFlagBits::eColorAttachment
        ); 
        
        VmaAllocationCreateInfo draw_image_allocinfo = {};
        draw_image_allocinfo.usage         = VMA_MEMORY_USAGE_GPU_ONLY;
        draw_image_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
        vkcheck(vmaCreateImage(
            m_VMA,
            reinterpret_cast<VkImageCreateInfo*>(&draw_image_ci),
            &draw_image_allocinfo,
            reinterpret_cast<VkImage*>(&m_DrawImage.img),
            &m_DrawImage.alloc,
            nullptr
        ), "failed to create draw image image (using VMA)");

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
            )
        );

        vkcheck(vkCreateImageView(
            m_Device.device,
            reinterpret_cast<VkImageViewCreateInfo*>(&draw_image_view_ci),
            allocator(),
            reinterpret_cast<VkImageView*>(&m_DrawImage.view)
        ), "failed to create draw image view");


        vkcheck(vkCreateCommandPool(
            m_Device.device,
            reinterpret_cast<VkCommandPoolCreateInfo*>(&command_pool_ci),
            allocator(),
            reinterpret_cast<VkCommandPool*>(&m_Immediate.pool)
        ), "failed to create immediate submit command pool");

        vk::CommandBufferAllocateInfo cmd_alloc_info(m_Immediate.pool, vk::CommandBufferLevel::ePrimary, 1);

        vkcheck(vkAllocateCommandBuffers(
            m_Device.device,
            reinterpret_cast<VkCommandBufferAllocateInfo*>(&cmd_alloc_info),
            reinterpret_cast<VkCommandBuffer*>(&m_Immediate.cmd)
        ), "failed to allocate immediate command buffer");


        return true;
    }
    
    auto Renderer::deinit() -> void {
        while (vkDeviceWaitIdle(m_Device.device) != VK_SUCCESS);

        vkDestroyCommandPool(m_Device.device, m_Immediate.pool, allocator());

        for (auto& render_pass : m_RenderPasses) {
            render_pass->destroy();
        }

        for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
            vkDestroyCommandPool(m_Device.device, m_Frames[i].pool, allocator());
            vkDestroyFence(m_Device, m_Frames[i].render_fence, allocator());
            vkDestroySemaphore(m_Device, m_Frames[i].acquire, allocator());
        }

        vkDestroyImageView(m_Device.device, m_DrawImage.view, allocator());
        vmaDestroyImage(m_VMA, m_DrawImage.img, m_DrawImage.alloc);

        vmaDestroyAllocator(m_VMA);

        vkb::destroy_swapchain(m_Swapchain);
        for (size_t i = 0; i < m_Images.size(); i++) {
            vkDestroyImageView(m_Device.device, m_Images[i].view, allocator());
            vkDestroySemaphore(m_Device, m_Images[i].render_finished, allocator());
        }

        vkb::destroy_surface(m_Instance, m_Surface);
        vkb::destroy_device(m_Device);
        vkb::destroy_instance(m_Instance);
    }
    
    auto Renderer::on_begin() -> bool {
        auto& frame = get_current_frame();

        vkcheck(vkWaitForFences(
            m_Device.device,
            1,
            reinterpret_cast<VkFence*>(&frame.render_fence),
            true,
            1000000000
        ), "failed to wait for render fence");

        vkcheck(vkResetFences(
            m_Device.device,
            1,
            reinterpret_cast<VkFence*>(&frame.render_fence)
        ), "failed to reset render fence");

        vkcheck(vkAcquireNextImageKHR(
            m_Device.device,
            m_Swapchain,
            1000000000,
            frame.acquire,
            nullptr,
            &m_SwapchainImgIndex
        ), "failed to acquire next swapchain image");

        vkcheck(vkResetCommandBuffer(frame.cmd, 0), "failed to reset command buffer");

        vk::CommandBufferBeginInfo cbbi(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        vkcheck(vkBeginCommandBuffer(
            frame.cmd,
            reinterpret_cast<VkCommandBufferBeginInfo*>(&cbbi)
        ), "failed to begin command buffer");

        transition_image(
            frame.cmd,
            m_DrawImage.img,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eGeneral
        );

        vk::ImageSubresourceRange clear_range(
            vk::ImageAspectFlagBits::eColor,
            0,
            VK_REMAINING_MIP_LEVELS,
            0,
            VK_REMAINING_ARRAY_LAYERS
        );

        clear_screen(frame.cmd, m_DrawImage.img, vk::ImageLayout::eGeneral, m_ClearColor, clear_range);

        auto swapchain_img = m_Images[m_SwapchainImgIndex].img;

        transition_image(
            frame.cmd,
            m_DrawImage.img,
            vk::ImageLayout::eGeneral,
            vk::ImageLayout::eColorAttachmentOptimal
        );

        // draw_geometry

        vk::RenderingAttachmentInfo color_attachment(
            m_DrawImage.view,
            vk::ImageLayout::eColorAttachmentOptimal,
            vk::ResolveModeFlagBits::eNone,
            {}, /* resolve image view */
            vk::ImageLayout::eUndefined,
            vk::AttachmentLoadOp::eLoad,
            vk::AttachmentStoreOp::eStore
        );

        vk::RenderingInfo render_info(
            vk::RenderingFlags{},
            vk::Rect2D(
                vk::Offset2D{},
                vk::Extent2D(m_DrawImage.extent.width, m_DrawImage.extent.height)
            ),
            1, /* layer count*/
            0, /* view mask */
            1, /* color attachment count */
            &color_attachment
        );

        vkCmdBeginRendering(
            frame.cmd,
            reinterpret_cast<VkRenderingInfo*>(&render_info)
        );

        for (auto& render_pass : m_RenderPasses) {
            render_pass->set_bind_point(vk::PipelineBindPoint::eGraphics);
            render_pass->set_cmd_buffer(frame.cmd);
            render_pass->bind();
            render_pass->set_viewport({
                static_cast<float>(m_DrawImage.extent.width),
                static_cast<float>(m_DrawImage.extent.height)
            });
            render_pass->set_scissor({0.f, 0.f}, {
                static_cast<float>(m_DrawImage.extent.width),
                static_cast<float>(m_DrawImage.extent.height)
            });
            render_pass->run();
        }

        // vkCmdDraw(frame.cmd, 3, 1, 0, 0);
        vkCmdEndRendering(frame.cmd);

        transition_image(
            frame.cmd,
            m_DrawImage.img, 
            vk::ImageLayout::eColorAttachmentOptimal,
            vk::ImageLayout::eTransferSrcOptimal
        );

        transition_image(
            frame.cmd,
            swapchain_img,
            vk::ImageLayout::eUndefined,
            vk::ImageLayout::eTransferDstOptimal
        );

        copy_image_to_image(
            frame.cmd,
            m_DrawImage.img,
            vk::Extent2D(m_DrawImage.extent.width, m_DrawImage.extent.height),
            swapchain_img,
            m_Swapchain.extent
        );

        transition_image(
            frame.cmd,
            swapchain_img, 
            vk::ImageLayout::eTransferDstOptimal,
            vk::ImageLayout::ePresentSrcKHR
        );
        
        return true;
    }
    
    auto Renderer::on_end() -> bool {
        auto& frame = get_current_frame();
        auto& img   = m_Images[m_SwapchainImgIndex];
        vkcheck(vkEndCommandBuffer(frame.cmd), "failed to end command buffer");
    
        vk::CommandBufferSubmitInfo cbsi(frame.cmd);
        
        vk::SemaphoreSubmitInfo wait_ssi(
            frame.acquire,
            1,
            vk::PipelineStageFlagBits2::eColorAttachmentOutputKHR
        );
        
        vk::SemaphoreSubmitInfo signal_ssi(
            img.render_finished,
            1,
            vk::PipelineStageFlagBits2::eAllGraphics
        );
        
        vk::SubmitInfo2 submit_info(
            {},
            1,
            &wait_ssi,
            1,
            &cbsi,
            1,
            &signal_ssi
        );

        vkQueueSubmit2(
            m_GraphicsQueue,
            1,
            reinterpret_cast<VkSubmitInfo2*>(&submit_info),
            frame.render_fence
        );
        vk::PresentInfoKHR present_info(
            1,
            &img.render_finished,
            1,
            reinterpret_cast<vk::SwapchainKHR*>(&m_Swapchain.swapchain),
            &m_SwapchainImgIndex
        );

        auto result = vkQueuePresentKHR(
            m_GraphicsQueue,
            reinterpret_cast<VkPresentInfoKHR*>(&present_info)
        );

        if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
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
        while (vkDeviceWaitIdle(m_Device.device) != VK_SUCCESS);

        vkb::SwapchainBuilder swapchain_builder(m_Device.physical_device, m_Device.device, m_Surface, m_GraphicsQueueFamily, m_PresentQueueFamily);
        auto swapchain_result = swapchain_builder.set_desired_extent(m_Width, m_Height)
            .set_desired_format(VkSurfaceFormatKHR{
                .format     = m_Swapchain.image_format,
                .colorSpace = m_Swapchain.color_space,
            })
            .set_allocation_callbacks(allocator())
            .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
            .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .add_image_usage_flags(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
            .set_desired_min_image_count(MAX_FRAMES_IN_FLIGHT)
            .set_old_swapchain(m_Swapchain)
            .build();

        vkbcheck(swapchain_result, "failed to recreate swapchain");

        m_Swapchain = swapchain_result.value();
        auto [imgs, views] = m_Swapchain.get_images_and_image_views().value();
        
        for (size_t i = 0; i < m_Images.size(); i++) {
            vkDestroyImageView(m_Device.device, m_Images[i].view, allocator());
        }
        
        for (size_t i = 0; i < imgs.size(); i++) {
            m_Images[i].img  = imgs[i];
            m_Images[i].view = views[i];
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

    auto Renderer::immediate_submit(std::function<void(vk::CommandBuffer)>&& fn) -> bool {
        vkcheck(vkResetFences(m_Device.device, 1, reinterpret_cast<VkFence*>(&m_Immediate.fence)),
            "failed to reset immediate submit fence");
        vkcheck(vkResetCommandBuffer(m_Immediate.cmd, 0), 
            "failed to reset immedaite submit command buffer");

        vk::CommandBufferBeginInfo begin_info(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
        vkcheck(vkBeginCommandBuffer(m_Immediate.cmd, reinterpret_cast<VkCommandBufferBeginInfo*>(&begin_info)), 
            "failed to begin immediate submit command buffer");

        fn(m_Immediate.cmd);

        vkcheck(vkEndCommandBuffer(m_Immediate.cmd), 
            "failed to end immediate submit command buffer");

        vk::CommandBufferSubmitInfo submit_info(m_Immediate.cmd);
        vk::SubmitInfo2 submit(vk::SubmitFlags(), 0, nullptr, 1, &submit_info);

        vkcheck(vkQueueSubmit2(
            m_GraphicsQueue,
            1, /* submit count */
            reinterpret_cast<VkSubmitInfo2*>(&submit),
            m_Immediate.fence
        ), "failed to submit immediate submit command buffer");

        vkcheck(vkWaitForFences(
            m_Device.device,
            1, /* fence count */
            reinterpret_cast<VkFence*>(&m_Immediate.fence),
            vk::True,
            9999999999 /* timeout */
        ), "failed to wait for immediate submit fence");

        return true;
    }


}
