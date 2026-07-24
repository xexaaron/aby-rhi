#pragma once
#include <VkBootstrap.h>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

    auto fence_create_info(vk::FenceCreateFlags flags) -> vk::FenceCreateInfo;
    auto semaphore_submit_info(vk::PipelineStageFlags2 stage_mask, vk::Semaphore semaphore) -> vk::SemaphoreSubmitInfo;
    auto semaphore_create_info(vk::SemaphoreCreateFlags flags) -> vk::SemaphoreCreateInfo;
    auto command_buffer_submit_info(vk::CommandBuffer cmd) -> vk::CommandBufferSubmitInfo;
    auto submit_info(vk::CommandBufferSubmitInfo& cmd, vk::SemaphoreSubmitInfo& signal_semaphore, vk::SemaphoreSubmitInfo& wait_semaphore) -> vk::SubmitInfo2;
    auto present_info(vkb::Swapchain& swapchain, vk::Semaphore& wait_semaphore, uint32_t* image_indices) -> vk::PresentInfoKHR;

    auto log_error(const std::string& message, const vkb::Error& error) -> void;
}