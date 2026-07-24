#include "backends/vulkan/vulkan-helpers.hpp"
#include "context.hpp"

namespace aby::rhi::vulkan {

    auto fence_create_info(vk::FenceCreateFlags flags) -> vk::FenceCreateInfo {
        vk::FenceCreateInfo ci(flags);
        return ci;
    }
    
    auto semaphore_submit_info(vk::PipelineStageFlags2 stage_mask, vk::Semaphore semaphore) -> vk::SemaphoreSubmitInfo {
        vk::SemaphoreSubmitInfo ssi(semaphore, UINT64_MAX, stage_mask);
        return ssi;
    }
    
    auto semaphore_create_info(vk::SemaphoreCreateFlags flags) -> vk::SemaphoreCreateInfo {
        vk::SemaphoreCreateInfo sci(flags);
        return sci;
    }
    
    auto command_buffer_submit_info(vk::CommandBuffer cmd) -> vk::CommandBufferSubmitInfo {
        vk::CommandBufferSubmitInfo cbsi(cmd);
        return cbsi;
    }
    
    auto submit_info(vk::CommandBufferSubmitInfo& cmd, vk::SemaphoreSubmitInfo& signal_semaphore, vk::SemaphoreSubmitInfo& wait_semaphore) -> vk::SubmitInfo2 {
        vk::SubmitInfo2 si2(vk::SubmitFlags(), 1, &wait_semaphore, 1, &cmd, 1, &signal_semaphore);
        return si2;
    }
    
    auto present_info(vkb::Swapchain& swapchain, vk::Semaphore& wait_semaphore, uint32_t* image_indices) -> vk::PresentInfoKHR {
        vk::SwapchainKHR swap(swapchain.swapchain);
        vk::PresentInfoKHR pikhr(1u, &wait_semaphore, 1u, &swap, image_indices);
        return pikhr;
    }

    auto log_error(const std::string& message, const vkb::Error& error) -> void {
        auto* log = Context::get().logger();
        log->log(ELogLevel::error, std::format("[vulkan] {}", message));
        for (size_t i = 0; i < error.detailed_failure_reasons.size(); i++) {
            auto msg = std::format("[vulkan] {}) {}", i, error.detailed_failure_reasons[i]);
            log->log(ELogLevel::error, msg);
        }
    }

}