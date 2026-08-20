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
		auto layout() -> vk::PipelineLayout;
		auto destroy() -> void;

		explicit operator vk::Pipeline();
		explicit operator VkPipeline();
	private:
		vk::Pipeline m_Pipeline;
		vk::PipelineLayout m_Layout;
		std::vector<vk::DescriptorSet> m_DescriptorSets;
	};

} // namespace aby::rhi::vulkan
