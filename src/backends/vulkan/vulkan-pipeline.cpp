#include "backends/vulkan/vulkan-pipeline.hpp"

#include "backends/vulkan/vulkan-common.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"
#include "context.hpp"

#include <array>

namespace aby::rhi::vulkan {

	Pipeline::Pipeline(vk::Pipeline pipeline, vk::PipelineLayout layout, const std::vector<vk::DescriptorSet>& sets) :
	    m_Pipeline(pipeline),
	    m_Layout(layout),
	    m_DescriptorSets(sets) {
	}

	auto Pipeline::bind(vk::CommandBuffer cmd, vk::PipelineBindPoint point) -> void {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
		vkCmdBindPipeline(cmd, static_cast<VkPipelineBindPoint>(point), m_Pipeline);

		for (auto& set : m_DescriptorSets) {
			vkCmdBindDescriptorSets(
			    cmd,
			    static_cast<VkPipelineBindPoint>(point),
			    m_Layout,
			    0, /* first set */
			    m_DescriptorSets.size(),
			    vkcast(m_DescriptorSets.data()),
			    0,      /* dynamic offset count */
			    nullptr /* dynamic offsets */
			);
		}
	}

	auto Pipeline::destroy() -> void {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
		vkDestroyPipelineLayout(r->device(), m_Layout, allocator());
		vkDestroyPipeline(r->device(), m_Pipeline, allocator());
	}

	Pipeline::operator vk::Pipeline() {
		return m_Pipeline;
	}

	Pipeline::operator VkPipeline() {
		return m_Pipeline;
	}

} // namespace aby::rhi::vulkan

