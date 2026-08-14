#pragma once
#include "common-enums.hpp"

#include <VkBootstrap.h>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

	auto transition_image(vk::CommandBuffer cmd, vk::Image image, vk::ImageLayout src, vk::ImageLayout dst,
	                      uint32_t mip_levels = VK_REMAINING_MIP_LEVELS, uint32_t base_mip = 0,
	                      uint32_t array_layers = VK_REMAINING_ARRAY_LAYERS, uint32_t base_layer = 0) -> void;
	auto copy_image_to_image(vk::CommandBuffer cmd, vk::Image src, vk::Extent2D src_sz, vk::Image dst, vk::Extent2D dst_sz) -> void;
	auto clear_screen(vk::CommandBuffer cmd, vk::Image image, vk::ImageLayout layout, vk::ClearColorValue& color, vk::ImageSubresourceRange& range) -> void;
	auto log_error(const std::string& message, const vkb::Error& error) -> void;

	auto eformat_to_vkformat(EFormat format) -> vk::Format;
	auto erepeatmode_to_vkrepeatmode(ERepeatMode repeat_mode) -> vk::SamplerAddressMode;
	auto efiltering_to_vkfilter(EFiltering filtering) -> std::pair<vk::Filter, vk::SamplerMipmapMode>;

} // namespace aby::rhi::vulkan
