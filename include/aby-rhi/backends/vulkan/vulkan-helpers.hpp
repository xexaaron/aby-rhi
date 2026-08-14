#pragma once
#include "common-enums.hpp"

#include <VkBootstrap.h>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

	auto log_error(const std::string& message, const vkb::Error& error) -> void;

	auto eformat_to_vkformat(EFormat format) -> vk::Format;
	auto erepeatmode_to_vkrepeatmode(ERepeatMode repeat_mode) -> vk::SamplerAddressMode;
	auto efiltering_to_vkfilter(EFiltering filtering) -> std::pair<vk::Filter, vk::SamplerMipmapMode>;
	auto eblendop_to_vkblendop(EBlendOp blend_op) -> vk::BlendOp;
	auto eblendfactor_to_vkblendfactor(EBlendFactor blend_factor) -> vk::BlendFactor;
	auto ecompareop_to_vkcompareop(ECompareOp compare_op) -> vk::CompareOp;
	auto ecullmode_to_vkcullmode(ECullMode cull_mode) -> vk::CullModeFlags;
	auto efrontface_to_vkfrontface(EFrontFace front_face) -> vk::FrontFace;
	auto etopology_to_vktopology(ETopology topology) -> vk::PrimitiveTopology;
	auto epolygonmode_to_vkpolygonmode(EPolygonMode polygon_mode) -> vk::PolygonMode;
	auto eshader_to_vkshader(EShader shader) -> vk::ShaderStageFlags;


} // namespace aby::rhi::vulkan
