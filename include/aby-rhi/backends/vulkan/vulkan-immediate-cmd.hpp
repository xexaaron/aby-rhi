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
