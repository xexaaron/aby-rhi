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
        void* m_pNext = nullptr;
        vk::DescriptorSetLayoutCreateFlags m_DescriptorFlags = {};
        vk::ShaderStageFlags m_StageFlags = {};
        std::vector<vk::DescriptorSetLayoutBinding> m_Bindings;
    };

    struct PoolSizeRatio {
        vk::DescriptorType type;
        float              ratio;
    };

    class DescriptorAllocator {
    public:
        auto init(uint32_t max_sets, std::span<PoolSizeRatio> pool_ratios) -> bool;
        auto alloc(vk::DescriptorSetLayout layout) -> vk::DescriptorSet;
        auto clear() -> bool;
        auto deinit() -> void; 
    private:
        vk::DescriptorPool m_Pool;
    };

}