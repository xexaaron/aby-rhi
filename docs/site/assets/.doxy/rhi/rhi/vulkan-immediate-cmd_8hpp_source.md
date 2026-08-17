

# File vulkan-immediate-cmd.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**backends**](dir_b2e089009e3d6019946b366985df1d18.md) **>** [**vulkan**](dir_8ecac3f6860031a9c88024a6c0dd2404.md) **>** [**vulkan-immediate-cmd.hpp**](vulkan-immediate-cmd_8hpp.md)

[Go to the documentation of this file](vulkan-immediate-cmd_8hpp.md)


```C++
#pragma once
#include "backends/vulkan/vulkan-common.hpp"

namespace aby::rhi::vulkan {

    class ImmediateCommands {
    public:
        ~ImmediateCommands();

        auto create(uint32_t queue_family) -> bool;
        auto destroy() -> void;

        auto begin() -> bool;
        auto end(vk::Queue queue) -> bool;

        auto cmd() -> vk::CommandBuffer;
    private:
        vk::Fence m_Fence       = VK_NULL_HANDLE;
        vk::CommandBuffer m_Cmd = VK_NULL_HANDLE;
        vk::CommandPool m_Pool  = VK_NULL_HANDLE;
        std::once_flag m_CreateFlag;
    };

} // namespace aby::rhi::vulkan
```


