#include "backends/vulkan/vulkan-immediate-cmd.hpp"

#include "backends/vulkan/vulkan-renderer.hpp"

namespace aby::rhi::vulkan {

	ImmediateCommands::~ImmediateCommands() {
		destroy();
	}

	auto ImmediateCommands::create(uint32_t queue_family) -> bool {
		std::call_once(m_CreateFlag, [&]() {
			auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

			vk::CommandPoolCreateInfo command_pool_ci(
			    vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
			    queue_family);

			vkassert(vkCreateCommandPool(
			             r->device(),
			             vkcast(command_pool_ci),
			             allocator(),
			             vkcast(m_Pool)),
			         "failed to create immediate submit command pool");

			vk::CommandBufferAllocateInfo cmd_alloc_info(
			    m_Pool,
			    vk::CommandBufferLevel::ePrimary,
			    1);

			vkassert(vkAllocateCommandBuffers(
			             r->device(),
			             vkcast(cmd_alloc_info),
			             vkcast(m_Cmd)),
			         "failed to allocate immediate submit command buffer");

			vk::FenceCreateInfo fence_ci(vk::FenceCreateFlagBits::eSignaled);

			vkassert(vkCreateFence(
			             r->device(),
			             vkcast(fence_ci),
			             allocator(),
			             vkcast(m_Fence)),
			         "failed to create immediate submit fence");
		});

		return true;
	}

	auto ImmediateCommands::destroy() -> void {
		if (m_Pool) {
			auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
			vkDestroyCommandPool(r->device(), m_Pool, allocator());
			vkDestroyFence(r->device(), m_Fence, allocator());
			m_Pool  = VK_NULL_HANDLE;
			m_Fence = VK_NULL_HANDLE;
			m_Cmd   = VK_NULL_HANDLE;
		}
	}

	auto ImmediateCommands::begin() -> bool {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
		vkcheck(vkResetFences(r->device(), 1, vkcast(m_Fence)), "failed to reset immediate submit fence");
		vkcheck(vkResetCommandBuffer(m_Cmd, 0), "failed to reset immedaite submit command buffer");
		vk::CommandBufferBeginInfo begin_info(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
		vkcheck(vkBeginCommandBuffer(m_Cmd, vkcast(begin_info)), "failed to begin immediate submit command buffer");
		return true;
	}

	auto ImmediateCommands::end(vk::Queue queue) -> bool {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
		vkcheck(vkEndCommandBuffer(m_Cmd), "failed to end immediate submit command buffer");

		vk::CommandBufferSubmitInfo submit_info(m_Cmd);
		vk::SubmitInfo2 submit(vk::SubmitFlags(), 0, nullptr, 1, &submit_info);

		vkcheck(vkQueueSubmit2(
		            queue,
		            1, /* submit count */
		            vkcast(submit),
		            m_Fence),
		        "failed to submit immediate submit command buffer");

		vkcheck(vkWaitForFences(
		            r->device(),
		            1, /* fence count */
		            vkcast(m_Fence),
		            vk::True,
		            9999999999 /* timeout */
		            ),
		        "failed to wait for immediate submit fence");

		return true;
	}

	auto ImmediateCommands::cmd() -> vk::CommandBuffer {
		return m_Cmd;
	}

} // namespace aby::rhi::vulkan
