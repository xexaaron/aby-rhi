

# File vulkan-platform.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**backends**](dir_b2e089009e3d6019946b366985df1d18.md) **>** [**vulkan**](dir_8ecac3f6860031a9c88024a6c0dd2404.md) **>** [**vulkan-platform.hpp**](vulkan-platform_8hpp.md)

[Go to the documentation of this file](vulkan-platform_8hpp.md)


```C++
#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

    auto create_surface(void* native_window, VkInstance instance, VkSurfaceKHR* surface) -> bool;
    auto get_instance_extensions(std::vector<const char*>* inst_exts) -> bool;
    auto get_device_extensions() -> std::vector<const char*>;
    auto get_extensions(std::vector<const char*>* inst_exts, std::vector<const char*>* dev_exts) -> bool;
    auto get_window_size(void* native_window, uint32_t* x, uint32_t* y) -> void;

} // namespace aby::rhi::vulkan
```


