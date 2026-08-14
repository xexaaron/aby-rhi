#include "backends/vulkan/vulkan-frame.hpp"

#include "backends/vulkan/vulkan-renderer.hpp"
#include "context.hpp"

namespace aby::rhi::vulkan {

	auto Frame::create(uint32_t graphics_queue_family) -> bool {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		vk::CommandPoolCreateInfo command_pool_ci(
		    vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
		    graphics_queue_family);

		vkcheck(vkCreateCommandPool(
		            r->device(),
		            vkcast(command_pool_ci),
		            allocator(),
		            vkcast(m_Pool)),
		        "failed to create command pool");

		vk::CommandBufferAllocateInfo cmd_alloc_info(m_Pool, vk::CommandBufferLevel::ePrimary, 1);

		vkcheck(vkAllocateCommandBuffers(
		            r->device(),
		            vkcast(cmd_alloc_info),
		            vkcast(m_Cmd)),
		        "failed to create command pool");

		vk::FenceCreateInfo fence_ci(vk::FenceCreateFlagBits::eSignaled);
		vkcheck(vkCreateFence(
		            r->device(),
		            vkcast(fence_ci),
		            allocator(),
		            vkcast(m_RenderFence)),
		        "failed to create render fence");

		vk::SemaphoreCreateInfo semaphore_ci;
		vkcheck(vkCreateSemaphore(
		            r->device(),
		            vkcast(semaphore_ci),
		            allocator(),
		            vkcast(m_Acquire)),
		        "failed to create wait semaphore");

		return true;
	}

	auto Frame::destroy() -> void {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
		if (m_Pool) {
			vkDestroyCommandPool(r->device(), m_Pool, allocator());
			m_Pool = VK_NULL_HANDLE;
		}
		if (m_RenderFence) {
			vkDestroyFence(r->device(), m_RenderFence, allocator());
			m_RenderFence = VK_NULL_HANDLE;
		}
		if (m_Acquire) {
			vkDestroySemaphore(r->device(), m_Acquire, allocator());
			m_Acquire = VK_NULL_HANDLE;
		}
	}

	auto Frame::begin(vk::SwapchainKHR swapchain, uint32_t* out_swapchain_index) -> bool {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
		vkassert(vkWaitForFences(
		             r->device(),
		             1,
		             vkcast(m_RenderFence),
		             true,
		             1000000000),
		         "failed to wait for render fence");

		vkcheck(vkResetFences(
		            r->device(),
		            1,
		            vkcast(m_RenderFence)),
		        "failed to reset render fence");

		vkcheck(vkAcquireNextImageKHR(
		            r->device(),
		            swapchain,
		            1000000000,
		            m_Acquire,
		            nullptr,
		            out_swapchain_index),
		        "failed to acquire next swapchain image");

		vkcheck(vkResetCommandBuffer(m_Cmd, 0), "failed to reset command buffer");

		vk::CommandBufferBeginInfo cbbi(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
		vkcheck(vkBeginCommandBuffer(
		            m_Cmd,
		            vkcast(cbbi)),
		        "failed to begin command buffer");

		return true;
	}

	auto Frame::end(vk::SwapchainKHR swapchain, vk::Queue queue, vk::Semaphore render_finished_semaphore, uint32_t swapchain_index) -> vk::Result {
		vkpropagate(vkEndCommandBuffer(m_Cmd), "failed to end command buffer");

		vk::CommandBufferSubmitInfo cbsi(m_Cmd);

		vk::SemaphoreSubmitInfo wait_ssi(
		    m_Acquire,
		    1,
		    vk::PipelineStageFlagBits2::eColorAttachmentOutputKHR);

		vk::SemaphoreSubmitInfo signal_ssi(
		    render_finished_semaphore,
		    1,
		    vk::PipelineStageFlagBits2::eAllGraphics);

		vk::SubmitInfo2 submit_info(
		    {},
		    1,
		    &wait_ssi,
		    1,
		    &cbsi,
		    1,
		    &signal_ssi);

		vkQueueSubmit2(queue, 1, vkcast(submit_info), m_RenderFence);
		vk::PresentInfoKHR present_info(
		    1,
		    &render_finished_semaphore,
		    1,
		    &swapchain,
		    &swapchain_index);

		return static_cast<vk::Result>(vkQueuePresentKHR(queue, vkcast(present_info)));
	}

	auto Frame::cmd() -> vk::CommandBuffer {
		return m_Cmd;
	}

} // namespace aby::rhi::vulkan

namespace aby::rhi::vulkan {

	auto Frames::create(uint32_t graphics_queue_family) -> bool {
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			if (!m_Frames[i].create(graphics_queue_family)) {
				return false;
			}
		}
		return true;
	}

	auto Frames::destroy() -> void {
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			m_Frames[i].destroy();
		}
	}

	auto Frames::operator->() -> Frame* {
		return &m_Frames[m_CurrentFrame % MAX_FRAMES_IN_FLIGHT];
	}

	auto Frames::operator++() -> Frames& {
		m_CurrentFrame++;
		return *this;
	}

	auto Frames::operator++(int) -> Frames& {
		m_CurrentFrame++;
		return *this;
	}

} // namespace aby::rhi::vulkan
