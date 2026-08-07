#pragma once
#include "backends/vulkan/vulkan-common.hpp"

namespace aby::rhi::vulkan {

	static constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;

	class Frame {
	public:
		Frame()  = default;
		~Frame() = default;

		auto create(uint32_t graphics_queue_family) -> bool;
		auto destroy() -> void;

		auto cmd() -> vk::CommandBuffer;

		auto begin(vk::SwapchainKHR swapchain, uint32_t* out_swapchain_index) -> bool;
		auto end(vk::SwapchainKHR swapchain, vk::Queue queue, vk::Semaphore render_finished_semaphore, uint32_t swapchain_index) -> vk::Result;
	private:
		vk::CommandPool m_Pool  = VK_NULL_HANDLE;
		vk::CommandBuffer m_Cmd = VK_NULL_HANDLE;
		vk::Semaphore m_Acquire = VK_NULL_HANDLE;
		vk::Fence m_RenderFence = VK_NULL_HANDLE;
	};

	class Frames {
	public:
		auto create(uint32_t graphics_queue_family) -> bool;
		auto destroy() -> void;

		auto operator->() -> Frame*;
		auto operator++() -> Frames&;
		auto operator++(int) -> Frames&;
	private:
		size_t m_CurrentFrame;
		std::array<Frame, MAX_FRAMES_IN_FLIGHT> m_Frames;
	};

} // namespace aby::rhi::vulkan
