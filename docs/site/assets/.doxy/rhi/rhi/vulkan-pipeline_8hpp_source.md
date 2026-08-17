

# File vulkan-pipeline.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**backends**](dir_b2e089009e3d6019946b366985df1d18.md) **>** [**vulkan**](dir_8ecac3f6860031a9c88024a6c0dd2404.md) **>** [**vulkan-pipeline.hpp**](vulkan-pipeline_8hpp.md)

[Go to the documentation of this file](vulkan-pipeline_8hpp.md)


```C++
#pragma once
#include "common.hpp"

#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

    class Pipeline {
    public:
        Pipeline(vk::Pipeline pipeline, vk::PipelineLayout layout, const std::vector<vk::DescriptorSet>& sets);

        auto bind(vk::CommandBuffer cmd, vk::PipelineBindPoint point) -> void;
        auto destroy() -> void;

        explicit operator vk::Pipeline();
        explicit operator VkPipeline();
    private:
        vk::Pipeline m_Pipeline;
        vk::PipelineLayout m_Layout;
        std::vector<vk::DescriptorSet> m_DescriptorSets;
    };

} // namespace aby::rhi::vulkan
```


