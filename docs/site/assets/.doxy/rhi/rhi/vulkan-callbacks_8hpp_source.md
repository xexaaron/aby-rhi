

# File vulkan-callbacks.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**backends**](dir_b2e089009e3d6019946b366985df1d18.md) **>** [**vulkan**](dir_8ecac3f6860031a9c88024a6c0dd2404.md) **>** [**vulkan-callbacks.hpp**](vulkan-callbacks_8hpp.md)

[Go to the documentation of this file](vulkan-callbacks_8hpp.md)


```C++
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
        void* user_data);

} // namespace aby::rhi::vulkan
```


