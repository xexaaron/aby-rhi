#include "backends/vulkan/vulkan-helpers.hpp"

#include "backends/vulkan/vulkan-common.hpp"
#include "context.hpp"

#include <vulkan/vk_enum_string_helper.h>

namespace aby::rhi::vulkan {

	auto log_error(const std::string& message, const vkb::Error& error) -> void {
		aby_rhi_err("[vulkan] {}: {}", message, string_VkResult(error.vk_result));
		for (size_t i = 0; i < error.detailed_failure_reasons.size(); i++) {
			aby_rhi_err("[vulkan] {}) {}", i, error.detailed_failure_reasons[i]);
		}
	}

	auto eformat_to_vkformat(EFormat format) -> vk::Format {
		switch (format) {
			case EFormat::none:
				return vk::Format::eUndefined;
			case EFormat::rgba_f32:
				return vk::Format::eR32G32B32A32Sfloat;
			case EFormat::rgb_f32:
				return vk::Format::eR32G32B32Sfloat;
			case EFormat::rg_f32:
				return vk::Format::eR32G32Sfloat;
			case EFormat::r_f32:
				return vk::Format::eR32Sfloat;
			case EFormat::rgba_i32:
				return vk::Format::eR32G32B32A32Sint;
			case EFormat::rgb_i32:
				return vk::Format::eR32G32B32Sint;
			case EFormat::rg_i32:
				return vk::Format::eR32G32Sint;
			case EFormat::r_i32:
				return vk::Format::eR32Sint;
			case EFormat::rgba_u32:
				return vk::Format::eR32G32B32A32Uint;
			case EFormat::rgb_u32:
				return vk::Format::eR32G32B32Uint;
			case EFormat::rg_u32:
				return vk::Format::eR32G32Uint;
			case EFormat::r_u32:
				return vk::Format::eR32Uint;
			default:
				aby_rhi_assert(false, "unknown EFormat -> vk::Format conversion");
				return vk::Format::eUndefined;
		}
	}

	auto erepeatmode_to_vkrepeatmode(ERepeatMode repeat_mode) -> vk::SamplerAddressMode {
		switch (repeat_mode) {
			case ERepeatMode::repeat:
				return vk::SamplerAddressMode::eRepeat;
				break;
			case ERepeatMode::mirrored:
				return vk::SamplerAddressMode::eMirroredRepeat;
				break;
			case ERepeatMode::clamp_to_edge:
				return vk::SamplerAddressMode::eClampToEdge;
				break;
			case ERepeatMode::clamp_to_border:
				return vk::SamplerAddressMode::eClampToBorder;
				break;
			case ERepeatMode::mirrored_clamp_to_edge:
				return vk::SamplerAddressMode::eMirrorClampToEdge;
				break;
			default:
				aby_rhi_assert(false, "unimplemented ERepeatMode in Vulkan backend: {}", repeat_mode);
		}
		return vk::SamplerAddressMode::eRepeat;
	}

	auto efiltering_to_vkfilter(EFiltering filtering) -> std::pair<vk::Filter, vk::SamplerMipmapMode> {
		switch (filtering) {
			case EFiltering::linear:
				return std::make_pair(vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear);
				break;
			case EFiltering::nearest:
				return std::make_pair(vk::Filter::eNearest, vk::SamplerMipmapMode::eNearest);
				break;
			case EFiltering::cubic:
				return std::make_pair(vk::Filter::eCubicIMG, vk::SamplerMipmapMode::eLinear);
				break;
			default:
				aby_rhi_assert(false, "unimplemented EFiltering mode in Vulkan backend: {}", filtering);
				break;
		}
		return std::make_pair(vk::Filter::eLinear, vk::SamplerMipmapMode::eLinear);
	}

	auto eblendop_to_vkblendop(EBlendOp blend_op) -> vk::BlendOp {
		switch (blend_op) {
			case EBlendOp::add:
				return vk::BlendOp::eAdd;
				break;
			case EBlendOp::sub:
				return vk::BlendOp::eSubtract;
				break;
			case EBlendOp::reverse_sub:
				return vk::BlendOp::eReverseSubtract;
				break;
			case EBlendOp::min:
				return vk::BlendOp::eMin;
				break;
			case EBlendOp::max:
				return vk::BlendOp::eMax;
				break;
			default:
				aby_rhi_assert(false, "unimplemented blend operation");
				break;
		}
		return vk::BlendOp::eAdd;
	}

	auto eblendfactor_to_vkblendfactor(EBlendFactor blend_factor) -> vk::BlendFactor {
		switch (blend_factor) {
			case EBlendFactor::zero:
				return vk::BlendFactor::eZero;
				break;
			case EBlendFactor::one:
				return vk::BlendFactor::eOne;
				break;
			case EBlendFactor::src_color:
				return vk::BlendFactor::eSrcColor;
				break;
			case EBlendFactor::one_minus_src_color:
				return vk::BlendFactor::eOneMinusSrcColor;
				break;
			case EBlendFactor::dst_color:
				return vk::BlendFactor::eDstColor;
				break;
			case EBlendFactor::one_minus_dst_color:
				return vk::BlendFactor::eOneMinusDstColor;
				break;
			case EBlendFactor::src_alpha:
				return vk::BlendFactor::eSrcAlpha;
				break;
			case EBlendFactor::one_minus_src_alpha:
				return vk::BlendFactor::eOneMinusSrcAlpha;
				break;
			case EBlendFactor::dst_alpha:
				return vk::BlendFactor::eDstAlpha;
				break;
			case EBlendFactor::one_minus_dst_alpha:
				return vk::BlendFactor::eOneMinusDstAlpha;
				break;
			case EBlendFactor::constant_color:
				return vk::BlendFactor::eConstantColor;
				break;
			case EBlendFactor::one_minus_constant_color:
				return vk::BlendFactor::eOneMinusConstantColor;
				break;
			case EBlendFactor::constant_alpha:
				return vk::BlendFactor::eConstantAlpha;
				break;
			case EBlendFactor::one_minus_constant_alpha:
				return vk::BlendFactor::eOneMinusConstantAlpha;
				break;
			case EBlendFactor::src_alpha_saturate:
				return vk::BlendFactor::eSrcAlphaSaturate;
				break;
			case EBlendFactor::src_one_color:
				return vk::BlendFactor::eSrc1Color;
				break;
			case EBlendFactor::one_minus_src_one_color:
				return vk::BlendFactor::eOneMinusSrc1Color;
				break;
			case EBlendFactor::src_one_alpha:
				return vk::BlendFactor::eSrc1Alpha;
				break;
			case EBlendFactor::one_minus_src_one_alpha:
				return vk::BlendFactor::eOneMinusSrc1Alpha;
				break;
			default:
				aby_rhi_assert(false, "unimplemented blend factor");
				break;
		}
		return vk::BlendFactor::eZero;
	}

	auto ecompareop_to_vkcompareop(ECompareOp compare_op) -> vk::CompareOp {
		switch (compare_op) {
			case ECompareOp::never:
				return vk::CompareOp::eNever;
				break;
			case ECompareOp::less:
				return vk::CompareOp::eLess;
				break;
			case ECompareOp::eq:
				return vk::CompareOp::eEqual;
				break;
			case ECompareOp::less_eq:
				return vk::CompareOp::eLessOrEqual;
				break;
			case ECompareOp::greater:
				return vk::CompareOp::eGreater;
				break;
			case ECompareOp::neq:
				return vk::CompareOp::eNotEqual;
				break;
			case ECompareOp::greater_eq:
				return vk::CompareOp::eGreaterOrEqual;
				break;
			case ECompareOp::always:
				return vk::CompareOp::eAlways;
				break;
			default:
				aby_rhi_assert(false, "unimplemented compare operation");
				break;
		}
		return vk::CompareOp::eNever;
	}

	auto ecullmode_to_vkcullmode(ECullMode cull_mode) -> vk::CullModeFlags {
		switch (cull_mode) {
			case ECullMode::none:
				return vk::CullModeFlagBits::eNone;
				break;
			case ECullMode::front:
				return vk::CullModeFlagBits::eFront;
				break;
			case ECullMode::back:
				return vk::CullModeFlagBits::eBack;
				break;
			case ECullMode::front_and_back:
				return vk::CullModeFlagBits::eFrontAndBack;
				break;
			default:
				aby_rhi_assert(false, "unimplemented cull mode");
				break;
		}
		return vk::CullModeFlagBits::eNone;
	}

	auto efrontface_to_vkfrontface(EFrontFace front_face) -> vk::FrontFace {
		switch (front_face) {
			case EFrontFace::clockwise:
				return vk::FrontFace::eClockwise;
				break;
			case EFrontFace::counter_clockwise:
				return vk::FrontFace::eCounterClockwise;
				break;
			default:
				aby_rhi_assert(false, "unimplemented front face");
				break;
		}
		return vk::FrontFace::eClockwise;
	}

	auto etopology_to_vktopology(ETopology topology) -> vk::PrimitiveTopology {
		switch (topology) {
			case ETopology::point_list:
				return vk::PrimitiveTopology::ePointList;
				break;
			case ETopology::line_list:
				return vk::PrimitiveTopology::eLineList;
				break;
			case ETopology::line_strip:
				return vk::PrimitiveTopology::eLineStrip;
				break;
			case ETopology::triangle_list:
				return vk::PrimitiveTopology::eTriangleList;
				break;
			case ETopology::triangle_strip:
				return vk::PrimitiveTopology::eTriangleStrip;
				break;
			case ETopology::triangle_fan:
				return vk::PrimitiveTopology::eTriangleFan;
				break;
			default:
				aby_rhi_assert(false, "unimplemented topology");
				break;
		}
		return vk::PrimitiveTopology::eTriangleList;
	}

	auto epolygonmode_to_vkpolygonmode(EPolygonMode polygon_mode) -> vk::PolygonMode {
		switch (polygon_mode) {
			case EPolygonMode::fill:
				return vk::PolygonMode::eFill;
				break;
			case EPolygonMode::line:
				return vk::PolygonMode::eLine;
				break;
			case EPolygonMode::point:
				return vk::PolygonMode::ePoint;
				break;
			default:
				aby_rhi_assert(false, "unimplemented polygon mode");
				break;
		}
		return vk::PolygonMode::eFill;
	}

	auto eshader_to_vkshader(EShader shader) -> vk::ShaderStageFlags {
		switch (shader) {
			case EShader::none:
				return vk::ShaderStageFlagBits::eAll;
				break;
			case EShader::vert:
				return vk::ShaderStageFlagBits::eVertex;
				break;
			case EShader::frag:
				return vk::ShaderStageFlagBits::eFragment;
				break;
			case EShader::comp:
				return vk::ShaderStageFlagBits::eCompute;
				break;
			case EShader::geom:
				return vk::ShaderStageFlagBits::eGeometry;
				break;
			default:
				aby_rhi_assert(false, "unimplemented shader stage");
		}
		return vk::ShaderStageFlagBits::eAll;
	}

} // namespace aby::rhi::vulkan
