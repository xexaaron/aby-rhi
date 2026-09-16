#pragma once
#include "common-enums.hpp"
#include "common.hpp"

#include <VkBootstrap.h>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

	auto ABY_RHI_API log_error(const std::string& message, const vkb::Error& error) -> void;

	auto ABY_RHI_API eformat_to_vkformat(EFormat format) -> vk::Format;
	auto ABY_RHI_API erepeatmode_to_vkrepeatmode(ERepeatMode repeat_mode) -> vk::SamplerAddressMode;
	auto ABY_RHI_API efiltering_to_vkfilter(EFiltering filtering) -> std::pair<vk::Filter, vk::SamplerMipmapMode>;
	auto ABY_RHI_API eblendop_to_vkblendop(EBlendOp blend_op) -> vk::BlendOp;
	auto ABY_RHI_API eblendfactor_to_vkblendfactor(EBlendFactor blend_factor) -> vk::BlendFactor;
	auto ABY_RHI_API ecompareop_to_vkcompareop(ECompareOp compare_op) -> vk::CompareOp;
	auto ABY_RHI_API ecullmode_to_vkcullmode(ECullMode cull_mode) -> vk::CullModeFlags;
	auto ABY_RHI_API efrontface_to_vkfrontface(EFrontFace front_face) -> vk::FrontFace;
	auto ABY_RHI_API etopology_to_vktopology(ETopology topology) -> vk::PrimitiveTopology;
	auto ABY_RHI_API epolygonmode_to_vkpolygonmode(EPolygonMode polygon_mode) -> vk::PolygonMode;
	auto ABY_RHI_API eshader_to_vkshader(EShader shader) -> vk::ShaderStageFlags;

} // namespace aby::rhi::vulkan
