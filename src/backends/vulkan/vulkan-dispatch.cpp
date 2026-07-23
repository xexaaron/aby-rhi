#include "backends/vulkan/vulkan-dispatch.hpp"
#include <vulkan/vulkan.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace aby::rhi::vulkan {

    auto load_functions(vk::Instance instance, vk::Device device) -> void {
        VULKAN_HPP_DEFAULT_DISPATCHER.init(
            instance,
            ::vkGetInstanceProcAddr,
            device,
            ::vkGetDeviceProcAddr
        );
    }

}
