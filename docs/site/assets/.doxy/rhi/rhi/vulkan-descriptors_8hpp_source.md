

# File vulkan-descriptors.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**backends**](dir_b2e089009e3d6019946b366985df1d18.md) **>** [**vulkan**](dir_8ecac3f6860031a9c88024a6c0dd2404.md) **>** [**vulkan-descriptors.hpp**](vulkan-descriptors_8hpp.md)

[Go to the documentation of this file](vulkan-descriptors_8hpp.md)


```C++
#pragma once
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

    class DescriptorLayoutBuilder {
    public:
        auto add_binding(uint32_t binding, vk::DescriptorType type) -> DescriptorLayoutBuilder&;
        auto set_flags(vk::DescriptorSetLayoutCreateFlags flags) -> DescriptorLayoutBuilder&;
        auto set_pnext(void* pnext) -> DescriptorLayoutBuilder&;
        auto set_stages(vk::ShaderStageFlags flags) -> DescriptorLayoutBuilder&;

        auto clear() -> void;
        auto build() -> vk::DescriptorSetLayout;
    private:
        void* m_pNext                                        = nullptr;
        vk::DescriptorSetLayoutCreateFlags m_DescriptorFlags = {};
        vk::ShaderStageFlags m_StageFlags                    = {};
        std::vector<vk::DescriptorSetLayoutBinding> m_Bindings;
    };

    struct PoolSizeRatio {
        vk::DescriptorType type;
        float ratio;
    };

    class DescriptorAllocator {
    public:
        auto init(uint32_t max_sets, std::span<PoolSizeRatio> pool_ratios, vk::DescriptorPoolCreateFlags flags) -> bool;
        auto alloc(vk::DescriptorSetLayout layout) -> vk::DescriptorSet;
        auto clear() -> bool;
        auto deinit() -> void;
    private:
        vk::DescriptorPool m_Pool;
    };

} // namespace aby::rhi::vulkan
```


