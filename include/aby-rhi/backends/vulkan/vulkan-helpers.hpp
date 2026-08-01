#pragma once
#include "common-enums.hpp"

#include <VkBootstrap.h>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

	auto transition_image(vk::CommandBuffer cmd, vk::Image image, vk::ImageLayout src, vk::ImageLayout dst) -> void;
	auto copy_image_to_image(vk::CommandBuffer cmd, vk::Image src, vk::Extent2D src_sz, vk::Image dst, vk::Extent2D dst_sz) -> void;
	auto clear_screen(vk::CommandBuffer cmd, vk::Image image, vk::ImageLayout layout, vk::ClearColorValue& color, vk::ImageSubresourceRange& range) -> void;
	auto log_error(const std::string& message, const vkb::Error& error) -> void;
	auto eformat_to_vkformat(EFormat format) -> vk::Format;

} // namespace aby::rhi::vulkan
