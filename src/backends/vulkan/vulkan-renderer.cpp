#include "backends/vulkan/vulkan-renderer.hpp"
#include "backends/vulkan/vulkan-callbacks.hpp"
#include "backends/vulkan/vulkan-platform.hpp"
#include "backends/vulkan/vulkan-helpers.hpp"
#include "context.hpp"
#include <vector>
#include <vulkan/vk_enum_string_helper.h>
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
        log->log(ELogLevel::debug, std::format("[vulkan] instance extensions: {}", instance_extensions.size()));
        for (size_t i = 0; i < instance_extensions.size(); i++) {
            std::string msg = std::format(" -- {}) {}", i, instance_extensions[i]);
            log->log(ELogLevel::debug, msg);
        }
#endif
     
        vkb::InstanceBuilder instb;
        auto inst_ret = instb.set_allocation_callbacks(allocator())
            .set_debug_callback(vk_debug_callback)
#ifndef NDEBUG
            .request_validation_layers(true)
#endif
            .enable_extensions(instance_extensions)
            .require_api_version(1, 4, 3)
            .build();

        if (!inst_ret.has_value()) {
            log_error("failed to create vulkan instance", inst_ret.full_error());
            return false;
        }

        m_Instance = inst_ret.value();

        VkSurfaceKHR surface = VK_NULL_HANDLE;
        if (!create_surface(native_window, m_Instance, &surface)) {
            log->log(ELogLevel::error, "[vulkan] failed to create surface");
            return false;
        }
        m_Surface = surface;

        VkPhysicalDeviceVulkan11Features features_11 { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES };
        VkPhysicalDeviceVulkan12Features features_12{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES };
        features_12.bufferDeviceAddress = true;
        features_12.descriptorIndexing = true;
        features_12.timelineSemaphore = true;
        VkPhysicalDeviceVulkan13Features features_13{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES };
        features_13.dynamicRendering = true;
        features_13.synchronization2 = true;
        VkPhysicalDeviceVulkan14Features features_14{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES };

        vkb::PhysicalDeviceSelector selector(inst_ret.value());
        auto gpu_ret = selector
            .set_minimum_version(1, 3)
            .set_required_features_14(features_14)
            .set_required_features_13(features_13)
            .set_required_features_12(features_12)
            .set_required_features_11(features_11)
            .set_surface(m_Surface)
            .add_required_extensions(device_extensions)
            .select();

        if (!gpu_ret.has_value()) {
            log_error("failed to find suitable gpu", gpu_ret.full_error());
            return false;
        }

        vkb::DeviceBuilder dev_builder{ gpu_ret.value() };
        auto device_ret = dev_builder
            .set_allocation_callbacks(allocator())
            .build();

        if (!device_ret.has_value()) {
            log_error("failed to create device", device_ret.full_error());
            return false;
        }
        
        m_Device              = device_ret.value();
        m_GraphicsQueue       = m_Device.get_queue(vkb::QueueType::graphics).value();
        m_GraphicsQueueFamily = m_Device.get_queue_index(vkb::QueueType::graphics).value();
        m_PresentQueue        = m_Device.get_queue(vkb::QueueType::present).value();
        m_PresentQueueFamily  = m_Device.get_queue_index(vkb::QueueType::present).value();

#ifndef NDEBUG
        VkPhysicalDeviceProperties props = {0};
        vkGetPhysicalDeviceProperties(m_Device.physical_device, &props);
        log->log(ELogLevel::debug, std::format("[vulkan] Found GPU: {}", props.deviceName));
#endif

        vkb::SwapchainBuilder swapchain_builder(m_Device.physical_device, m_Device.device, m_Surface, m_GraphicsQueueFamily, m_PresentQueueFamily);
        auto swapchain_result = swapchain_builder.set_desired_extent(800, 600)
            .set_desired_format(VkSurfaceFormatKHR{.format = VK_FORMAT_R8G8B8A8_UNORM })
            .set_allocation_callbacks(allocator())
            .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
            .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .add_image_usage_flags(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
            .set_desired_min_image_count(2)
            .build();

        if (!swapchain_result.has_value()) {
            log_error("failed to create swapchain", swapchain_result.full_error());
            return false;
        }   

        m_Swapchain = swapchain_result.value();


        return true;
    }
    
    auto Renderer::deinit() -> void {
        vkb::destroy_swapchain(m_Swapchain);
        vkb::destroy_surface(m_Instance, m_Surface);
        vkb::destroy_device(m_Device);
        vkb::destroy_instance(m_Instance);
    }
    
    auto Renderer::on_begin() -> bool {
        return true;
    }
    
    auto Renderer::on_end() -> void {

    }
    
    auto Renderer::on_resize(uint32_t x, uint32_t y) -> void {
        vkb::SwapchainBuilder swapchain_builder(m_Device.physical_device, m_Device.device, m_Surface, m_GraphicsQueueFamily, m_PresentQueueFamily);
        auto swapchain_result = swapchain_builder.set_desired_extent(x, y)
            .set_desired_format(VkSurfaceFormatKHR{
                .format     = m_Swapchain.image_format,
                .colorSpace = m_Swapchain.color_space,
            })
            .set_allocation_callbacks(allocator())
            .set_desired_present_mode(VK_PRESENT_MODE_FIFO_KHR)
            .add_image_usage_flags(VK_IMAGE_USAGE_TRANSFER_DST_BIT)
            .add_image_usage_flags(VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT)
            .set_desired_min_image_count(2)
            .set_old_swapchain(m_Swapchain)
            .build();

        if (!swapchain_result.has_value()) {
            log_error("failed to (re)create swapchain", swapchain_result.full_error());
            return;
        }

        m_Swapchain = swapchain_result.value();
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


}
