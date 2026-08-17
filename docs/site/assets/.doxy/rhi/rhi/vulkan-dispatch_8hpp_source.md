

# File vulkan-dispatch.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**backends**](dir_b2e089009e3d6019946b366985df1d18.md) **>** [**vulkan**](dir_8ecac3f6860031a9c88024a6c0dd2404.md) **>** [**vulkan-dispatch.hpp**](vulkan-dispatch_8hpp.md)

[Go to the documentation of this file](vulkan-dispatch_8hpp.md)


```C++
#pragma once
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

    auto load_functions(vk::Instance instance, vk::Device device) -> void;

}
```


