#pragma once
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

    auto allocator() -> VkAllocationCallbacks*;
    
    auto VKAPI_PTR vk_alloc_callback(void* pUserData, size_t size, size_t alignment, VkSystemAllocationScope allocationScope) -> void*;
    auto VKAPI_PTR vk_realloc_callback(void* pUserData, void* pOriginal, size_t size, size_t alignment, VkSystemAllocationScope allocationScope) -> void*;
    auto VKAPI_PTR vk_free_callback(void* pUserData, void* pMemory) -> void;
    VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT severity,
        VkDebugUtilsMessageTypeFlagsEXT type,
        const VkDebugUtilsMessengerCallbackDataEXT* data,
        void* user_data
    );

}