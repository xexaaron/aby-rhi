#include "backends/vulkan/vulkan-texture.hpp"

#include "backends/vulkan/vulkan-common.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"

#include <stb_image/stb_image.h>
#include <stb_image/stb_image_resize2.h>
#include <stb_image/stb_image_write.h>

namespace aby::rhi::vulkan::detail {

	constexpr auto unorm_to_u8(float value) -> uint8_t {
		value = std::clamp(value, 0.0f, 1.0f);
		return static_cast<uint8_t>(std::round(value * 255.0f));
	}

	constexpr auto snorm_to_u8(float value) -> uint8_t {
		value = std::clamp(value, -1.0f, 1.0f);
		return unorm_to_u8(value * 0.5f + 0.5f);
	}

	constexpr auto uint_to_u8(uint8_t value) -> uint8_t {
		return value;
	}

	constexpr auto uint_to_u8(uint16_t value) -> uint8_t {
		return static_cast<uint8_t>(
		    (static_cast<uint32_t>(value) * 255u) / 65535u);
	}

	constexpr auto uint_to_u8(uint32_t value) -> uint8_t {
		return static_cast<uint8_t>(
		    (static_cast<uint64_t>(value) * 255ull) / 4294967295ull);
	}

	constexpr auto sint_to_u8(int8_t value) -> uint8_t {
		const auto normalized =
		    std::clamp(static_cast<float>(value) / 127.0f, -1.0f, 1.0f);

		return snorm_to_u8(normalized);
	}

	constexpr auto sint_to_u8(int16_t value) -> uint8_t {
		const auto normalized =
		    std::clamp(static_cast<float>(value) / 32767.0f, -1.0f, 1.0f);

		return snorm_to_u8(normalized);
	}

	constexpr auto sint_to_u8(int32_t value) -> uint8_t {
		const auto normalized =
		    std::clamp(static_cast<float>(value) / 2147483647.0f, -1.0f, 1.0f);

		return snorm_to_u8(normalized);
	}

	constexpr auto float_to_u8(float value) -> uint8_t {
		return unorm_to_u8(value);
	}

	auto half_to_float(uint16_t value) -> float {
		const uint32_t sign = (value & 0x8000u) << 16;
		const uint32_t exp  = (value >> 10) & 0x1Fu;
		const uint32_t mant = value & 0x03FFu;

		uint32_t result;

		if (exp == 0) {
			if (mant == 0) {
				result = sign;
			} else {
				uint32_t m = mant;
				int e      = -1;

				do {
					m <<= 1;
					--e;
				} while ((m & 0x0400u) == 0);

				m &= 0x03FFu;

				const uint32_t new_exp =
				    static_cast<uint32_t>(e + 127);

				result = sign | (new_exp << 23) | (m << 13);
			}
		} else if (exp == 0x1Fu) {
			result = sign | 0x7F800000u | (mant << 13);
		} else {
			const uint32_t new_exp = exp + (127 - 15);

			result = sign |
			         (new_exp << 23) |
			         (mant << 13);
		}

		float result_float;
		std::memcpy(&result_float, &result, sizeof(result_float));

		return result_float;
	}

	constexpr auto snorm8_to_u8(int8_t value) -> uint8_t {
		return snorm_to_u8(
		    std::max(-1.0f, static_cast<float>(value) / 127.0f));
	}

	constexpr auto snorm16_to_u8(int16_t value) -> uint8_t {
		return snorm_to_u8(
		    std::max(-1.0f, static_cast<float>(value) / 32767.0f));
	}

} // namespace aby::rhi::vulkan::detail

namespace aby::rhi::vulkan {

	Texture::Texture(ResourceID id, const fs::path& path, const TextureParams& params) :
	    m_ID(INVALID_ID),
	    m_Channels(std::popcount(static_cast<uint8_t>(params.channels))),
	    m_Sampler(VK_NULL_HANDLE),
	    bIsRenderTarget(false),
	    m_FrameID(SIZE_MAX),
	    m_Samples(vk::SampleCountFlagBits::e1) {
		uint32_t w, h;
		if (!load(path, &w, &h)) {
			return;
		}

		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		uint32_t max_mip_levels = 1 + static_cast<uint32_t>(std::floor(std::log2(std::max(w, h))));
		uint32_t mip_levels     = params.mip_levels == 0
		                            ? max_mip_levels
		                            : std::min(params.mip_levels, max_mip_levels);

		vk::Format img_format;
		switch (params.texture_usage) {
			case ETextureUsage::albedo: {
				switch (m_Channels) {
					case 1:
						img_format = vk::Format::eR8Srgb;
						break;
					case 2:
						img_format = vk::Format::eR8G8Srgb;
						break;
					case 3:
						img_format = vk::Format::eR8G8B8Srgb;
						break;
					case 4:
						img_format = vk::Format::eR8G8B8A8Srgb;
						break;
				}
				break;
			}
			case ETextureUsage::material: {
				switch (m_Channels) {
					case 1:
						img_format = vk::Format::eR8Unorm;
						break;
					case 2:
						img_format = vk::Format::eR8G8Unorm;
						break;
					case 3:
						img_format = vk::Format::eR8G8B8Unorm;
						break;
					case 4:
						img_format = vk::Format::eR8G8B8A8Unorm;
						break;
				}
				break;
			}
		}

		if (!m_Image.create(
		        vk::Extent3D(w, h, 1),
		        img_format,
		        m_Samples,
		        vk::ImageUsageFlagBits::eTransferDst |
		            vk::ImageUsageFlagBits::eTransferSrc |
		            vk::ImageUsageFlagBits::eSampled,
		        mip_levels)) {
			aby_rhi_err("failed to create image for texture: {}", path.string());
		}

		auto [filter_mode, sampler_mipmap_mode] = efiltering_to_vkfilter(params.filtering);
		auto repeat_mode                        = erepeatmode_to_vkrepeatmode(params.repeat_mode);

		vk::Bool32 anisotropy_enable = (params.anisotropy_filtering == 0.f ? vk::False : vk::True);
		float max_aniostropy         = anisotropy_enable ? std::min<float>(params.anisotropy_filtering, r->max_sampler_anisotropy()) : 1.f;

		vk::SamplerCreateInfo sampler_create_info(
		    vk::SamplerCreateFlags(),
		    filter_mode,                             /* mag filter */
		    filter_mode,                             /* min filter */
		    sampler_mipmap_mode,                     /* mipmap mode */
		    repeat_mode,                             /* u */
		    repeat_mode,                             /* v */
		    repeat_mode,                             /* w */
		    0.f,                                     /* mip load bias */
		    anisotropy_enable,                       /* anisotropy enable */
		    max_aniostropy,                          /* max anisotropy */
		    vk::False,                               /* compare enable */
		    vk::CompareOp::eNever,                   /* compare operator */
		    0.f,                                     /* min lod*/
		    mip_levels,                              /* max lod*/
		    vk::BorderColor::eFloatTransparentBlack, /* border color */
		    vk::False                                /* unormalized coordinates */
		);

		vkassert(vkCreateSampler(
		             r->device(),
		             vkcast(sampler_create_info),
		             allocator(),
		             vkcast(m_Sampler)),
		         "failed to create texture sampler for: {}", path.string());

		auto staging = Buffer(
		    this->bytes(),
		    vk::BufferUsageFlagBits::eTransferSrc,
		    VMA_MEMORY_USAGE_CPU_ONLY);

		staging.write(m_Data.data(), m_Data.size());

		r->immediate_submit([&](vk::CommandBuffer cmd) {
			// All mips -> transfer destination.
			m_Image.transition(cmd, vk::ImageLayout::eTransferDstOptimal);

			// Upload mip 0.
			m_Image.copy_from(cmd, staging, 0);

			int32_t mip_width  = static_cast<int32_t>(w);
			int32_t mip_height = static_cast<int32_t>(h);

			for (uint32_t i = 1; i < mip_levels; ++i) {
				// mip i-1: transfer destination -> transfer source
				m_Image.transition(cmd, vk::ImageLayout::eTransferSrcOptimal, 1, i - 1);
				// Blit mip i-1 -> mip i.
				// mip i is already TRANSFER_DST_OPTIMAL.
				m_Image.copy_to(cmd, mip_width, mip_height, i, filter_mode);
				// mip i-1: transfer source -> shader read.
				m_Image.transition(cmd, vk::ImageLayout::eShaderReadOnlyOptimal, 1, i - 1);
				mip_width  = std::max(1, mip_width / 2);
				mip_height = std::max(1, mip_height / 2);
			}

			// Final mip: transfer destination -> shader read.
			m_Image.transition(cmd, vk::ImageLayout::eShaderReadOnlyOptimal, 1, mip_levels - 1);
			return true;
		});

		m_ID = r->register_texture(id, m_Image.view(), m_Sampler);
	}

	Texture::Texture(ResourceID id, uint32_t width, uint32_t height, uint8_t channels, vk::SampleCountFlagBits samples) :
	    m_ID(INVALID_ID),
	    m_Channels(channels),
	    m_Sampler(VK_NULL_HANDLE),
	    bIsRenderTarget(true),
	    m_FrameID(SIZE_MAX),
	    m_Samples(samples) {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		if (!m_Image.create(
		        vk::Extent3D(width, height, 1),
		        r->color_format(),
		        m_Samples,
		        vk::ImageUsageFlagBits::eTransferDst |
		            vk::ImageUsageFlagBits::eTransferSrc |
		            vk::ImageUsageFlagBits::eSampled |
		            vk::ImageUsageFlagBits::eColorAttachment)) {
			aby_rhi_err("failed to create image for texture: {}", id);
		}

		vk::SamplerCreateInfo sampler_create_info(
		    vk::SamplerCreateFlags(),
		    vk::Filter::eLinear,                     /* mag filter */
		    vk::Filter::eLinear,                     /* min filter */
		    vk::SamplerMipmapMode::eLinear,          /* mipmap mode */
		    vk::SamplerAddressMode::eClampToEdge,    /* u */
		    vk::SamplerAddressMode::eClampToEdge,    /* v */
		    vk::SamplerAddressMode::eClampToEdge,    /* w */
		    0.f,                                     /* mip load bias */
		    vk::False,                               /* anisotropy enable */
		    0.f,                                     /* max anisotropy */
		    vk::False,                               /* compare enable */
		    vk::CompareOp::eNever,                   /* compare operator */
		    0.f,                                     /* min lod*/
		    0.f,                                     /* max lod*/
		    vk::BorderColor::eFloatTransparentBlack, /* border color */
		    vk::False                                /* unormalized coordinates */
		);

		vkassert(vkCreateSampler(
		             r->device(),
		             vkcast(sampler_create_info),
		             allocator(),
		             vkcast(m_Sampler)),
		         "failed to create texture sampler");

		m_ID = r->register_texture(id, m_Image.view(), m_Sampler);
	}

	Texture::~Texture() {
		destroy();
	}

	auto Texture::destroy() -> void {
		m_Image.destroy();
		if (m_Sampler) {
			auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
			vkDestroySampler(r->device(), m_Sampler, allocator());
			m_Sampler = VK_NULL_HANDLE;
		}
		m_Data.clear();
	}

	auto Texture::sync() -> void {
		if (!bIsRenderTarget) {
			return;
		}

		auto* r                   = static_cast<vulkan::Renderer*>(Context::get().renderer());
		auto render_target_format = r->color_format();

		Image& image = m_Samples != vk::SampleCountFlagBits::e1
		                 ? static_cast<vulkan::Texture*>(r->get_resolve_attachment(this))->image()
		                 : m_Image;

		auto prev_layout = image.layout();

		const auto size = width() * height() * image.bpp();
		Buffer staging(size, vk::BufferUsageFlagBits::eTransferDst, VMA_MEMORY_USAGE_CPU_ONLY);

		r->immediate_submit([&image, &staging, prev_layout](vk::CommandBuffer cmd) {
			image.transition(cmd, vk::ImageLayout::eTransferSrcOptimal);
			image.copy_to(cmd, staging, 0);
			image.transition(cmd, prev_layout);
		});

		staging.read(&m_Data);

		m_FrameID = r->frame_index();
	}

	auto Texture::read_px(uint32_t x, uint32_t y) -> vec4<uint8_t> {
		if (bIsRenderTarget) {
			auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

			aby_rhi_assert(
			    m_FrameID == r->frame_index(),
			    "attempting to read px from a render target but the texture "
			    "was not synchronized for the current frame");
		}

		const auto width  = m_Image.width();
		const auto height = m_Image.height();

		aby_rhi_assert(
		    x < width && y < height,
		    "texture pixel coordinates out of range: ({}, {}) for {}x{}",
		    x, y, width, height);

		const auto format = m_Image.format();

		vec4<uint8_t> color{ 0, 0, 0, 255 };

		switch (format) {
			case vk::Format::eR8Unorm: {
				const auto pixel = (y * width + x);
				color.x          = m_Data[pixel];
				color.y          = color.x;
				color.z          = color.x;
				break;
			}
			case vk::Format::eR8G8Unorm: {
				const auto pixel = (y * width + x) * 2;
				color.x          = m_Data[pixel + 0];
				color.y          = m_Data[pixel + 1];
				break;
			}
			case vk::Format::eR8G8B8Unorm: {
				const auto pixel = (y * width + x) * 3;
				color.x          = m_Data[pixel + 0];
				color.y          = m_Data[pixel + 1];
				color.z          = m_Data[pixel + 2];
				break;
			}
			case vk::Format::eR8G8B8A8Unorm: {
				const auto pixel = (y * width + x) * 4;
				color.x          = m_Data[pixel + 0];
				color.y          = m_Data[pixel + 1];
				color.z          = m_Data[pixel + 2];
				color.w          = m_Data[pixel + 3];
				break;
			}
			case vk::Format::eB8G8R8A8Unorm: {
				const auto pixel = (y * width + x) * 4;
				color.x          = m_Data[pixel + 2];
				color.y          = m_Data[pixel + 1];
				color.z          = m_Data[pixel + 0];
				color.w          = m_Data[pixel + 3];
				break;
			}
			case vk::Format::eR16Unorm: {
				const auto pixel = (y * width + x) * 2;
				uint16_t value;
				std::memcpy(&value, m_Data.data() + pixel, sizeof(value));
				color.x = detail::unorm_to_u8(static_cast<float>(value) / 65535.0f);
				color.y = color.x;
				color.z = color.x;
				break;
			}
			case vk::Format::eR16G16Unorm: {
				const auto pixel = (y * width + x) * 4;

				uint16_t values[2];
				std::memcpy(values, m_Data.data() + pixel, sizeof(values));
				color.x = detail::unorm_to_u8(static_cast<float>(values[0]) / 65535.0f);
				color.y = detail::unorm_to_u8(static_cast<float>(values[1]) / 65535.0f);
				break;
			}
			case vk::Format::eR8Snorm: {
				const auto pixel = y * width + x;
				color.x          = detail::snorm8_to_u8(std::bit_cast<int8_t>(m_Data[pixel]));
				color.y          = color.x;
				color.z          = color.x;
				break;
			}
			case vk::Format::eR8G8Snorm: {
				const auto pixel = (y * width + x) * 2;
				color.x          = detail::snorm8_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 0]));
				color.y          = detail::snorm8_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 1]));
				break;
			}
			case vk::Format::eR8G8B8Snorm: {
				const auto pixel = (y * width + x) * 3;
				color.x          = detail::snorm8_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 0]));
				color.y          = detail::snorm8_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 1]));
				color.z          = detail::snorm8_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 2]));
				break;
			}
			case vk::Format::eR8G8B8A8Snorm: {
				const auto pixel = (y * width + x) * 4;
				color.x          = detail::snorm8_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 0]));
				color.y          = detail::snorm8_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 1]));
				color.z          = detail::snorm8_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 2]));
				color.w          = detail::snorm8_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 3]));
				break;
			}
			case vk::Format::eR16Snorm: {
				const auto pixel = (y * width + x) * 2;
				int16_t value;
				std::memcpy(&value, m_Data.data() + pixel, sizeof(value));
				color.x = detail::snorm16_to_u8(value);
				color.y = color.x;
				color.z = color.x;
				break;
			}
			case vk::Format::eR16G16Snorm: {
				const auto pixel = (y * width + x) * 4;
				int16_t values[2];
				std::memcpy(values, m_Data.data() + pixel, sizeof(values));
				color.x = detail::snorm16_to_u8(values[0]);
				color.y = detail::snorm16_to_u8(values[1]);
				break;
			}
			case vk::Format::eR8Uint: {
				const auto pixel = y * width + x;
				color.x          = detail::uint_to_u8(std::bit_cast<uint8_t>(m_Data[pixel]));
				color.y          = color.x;
				color.z          = color.x;
				break;
			}
			case vk::Format::eR8G8Uint: {
				const auto pixel = (y * width + x) * 2;
				color.x          = detail::uint_to_u8(std::bit_cast<uint8_t>(m_Data[pixel + 0]));
				color.y          = detail::uint_to_u8(std::bit_cast<uint8_t>(m_Data[pixel + 1]));
				break;
			}
			case vk::Format::eR8G8B8Uint: {
				const auto pixel = (y * width + x) * 3;
				color.x          = detail::uint_to_u8(std::bit_cast<uint8_t>(m_Data[pixel + 0]));
				color.y          = detail::uint_to_u8(std::bit_cast<uint8_t>(m_Data[pixel + 1]));
				color.z          = detail::uint_to_u8(std::bit_cast<uint8_t>(m_Data[pixel + 2]));
				break;
			}
			case vk::Format::eR8G8B8A8Uint: {
				const auto pixel = (y * width + x) * 4;
				color.x          = m_Data[pixel + 0];
				color.y          = m_Data[pixel + 1];
				color.z          = m_Data[pixel + 2];
				color.w          = m_Data[pixel + 3];
				break;
			}
			case vk::Format::eR16Uint: {
				const auto pixel = (y * width + x) * 2;
				uint16_t value;
				std::memcpy(&value, m_Data.data() + pixel, sizeof(value));
				color.x = detail::uint_to_u8(value);
				color.y = color.x;
				color.z = color.x;
				break;
			}
			case vk::Format::eR16G16Uint: {
				const auto pixel = (y * width + x) * 4;
				uint16_t values[2];
				std::memcpy(values, m_Data.data() + pixel, sizeof(values));
				color.x = detail::uint_to_u8(values[0]);
				color.y = detail::uint_to_u8(values[1]);
				break;
			}
			case vk::Format::eR16G16B16A16Uint: {
				const auto pixel = (y * width + x) * 8;
				uint16_t values[4];
				std::memcpy(values, m_Data.data() + pixel, sizeof(values));
				color.x = detail::uint_to_u8(values[0]);
				color.y = detail::uint_to_u8(values[1]);
				color.z = detail::uint_to_u8(values[2]);
				color.w = detail::uint_to_u8(values[3]);
				break;
			}
			case vk::Format::eR32Uint: {
				const auto pixel = (y * width + x) * 4;
				uint32_t value;
				std::memcpy(&value, m_Data.data() + pixel, sizeof(value));
				color.x = detail::uint_to_u8(value);
				color.y = color.x;
				color.z = color.x;
				break;
			}
			case vk::Format::eR32G32B32A32Uint: {
				const auto pixel = (y * width + x) * 16;
				uint32_t values[4];
				std::memcpy(values, m_Data.data() + pixel, sizeof(values));
				color.x = detail::uint_to_u8(values[0]);
				color.y = detail::uint_to_u8(values[1]);
				color.z = detail::uint_to_u8(values[2]);
				color.w = detail::uint_to_u8(values[3]);
				break;
			}
			case vk::Format::eR8Sint: {
				const auto pixel = y * width + x;
				color.x          = detail::sint_to_u8(std::bit_cast<int8_t>(m_Data[pixel]));
				color.y          = color.x;
				color.z          = color.x;
				break;
			}
			case vk::Format::eR8G8Sint: {
				const auto pixel = (y * width + x) * 2;
				color.x          = detail::sint_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 0]));
				color.y          = detail::sint_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 1]));
				break;
			}
			case vk::Format::eR8G8B8Sint: {
				const auto pixel = (y * width + x) * 3;
				color.x          = detail::sint_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 0]));
				color.y          = detail::sint_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 1]));
				color.z          = detail::sint_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 2]));
				break;
			}
			case vk::Format::eR8G8B8A8Sint: {
				const auto pixel = (y * width + x) * 4;
				color.x          = detail::sint_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 0]));
				color.y          = detail::sint_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 1]));
				color.z          = detail::sint_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 2]));
				color.w          = detail::sint_to_u8(std::bit_cast<int8_t>(m_Data[pixel + 3]));
				break;
			}
			case vk::Format::eR16Sint: {
				const auto pixel = (y * width + x) * 2;
				int16_t value;
				std::memcpy(&value, m_Data.data() + pixel, sizeof(value));
				color.x = detail::sint_to_u8(value);
				color.y = color.x;
				color.z = color.x;
				break;
			}
			case vk::Format::eR16G16Sint: {
				const auto pixel = (y * width + x) * 4;
				int16_t values[2];
				std::memcpy(values, m_Data.data() + pixel, sizeof(values));
				color.x = detail::sint_to_u8(values[0]);
				color.y = detail::sint_to_u8(values[1]);
				break;
			}
			case vk::Format::eR16G16B16A16Sint: {
				const auto pixel = (y * width + x) * 8;

				int16_t values[4];
				std::memcpy(values, m_Data.data() + pixel, sizeof(values));

				color.x = detail::sint_to_u8(values[0]);
				color.y = detail::sint_to_u8(values[1]);
				color.z = detail::sint_to_u8(values[2]);
				color.w = detail::sint_to_u8(values[3]);
				break;
			}
			case vk::Format::eR32Sint: {
				const auto pixel = (y * width + x) * 4;
				int32_t value;
				std::memcpy(&value, m_Data.data() + pixel, sizeof(value));
				color.x = detail::sint_to_u8(value);
				color.y = color.x;
				color.z = color.x;
				break;
			}
			case vk::Format::eR32G32B32A32Sint: {
				const auto pixel = (y * width + x) * 16;
				int32_t values[4];
				std::memcpy(values, m_Data.data() + pixel, sizeof(values));

				color.x = detail::sint_to_u8(values[0]);
				color.y = detail::sint_to_u8(values[1]);
				color.z = detail::sint_to_u8(values[2]);
				color.w = detail::sint_to_u8(values[3]);
				break;
			}
			case vk::Format::eR16Sfloat: {
				const auto pixel = (y * width + x) * 2;
				uint16_t value;
				std::memcpy(&value, m_Data.data() + pixel, sizeof(value));
				color.x = detail::float_to_u8(detail::half_to_float(value));
				color.y = color.x;
				color.z = color.x;
				break;
			}
			case vk::Format::eR16G16Sfloat: {
				const auto pixel = (y * width + x) * 4;
				uint16_t values[2];
				std::memcpy(values, m_Data.data() + pixel, sizeof(values));
				color.x = detail::float_to_u8(detail::half_to_float(values[0]));
				color.y = detail::float_to_u8(detail::half_to_float(values[1]));
				break;
			}
			case vk::Format::eR16G16B16A16Sfloat: {
				const auto pixel = (y * width + x) * 8;
				uint16_t values[4];
				std::memcpy(values, m_Data.data() + pixel, sizeof(values));
				color.x = detail::float_to_u8(detail::half_to_float(values[0]));
				color.y = detail::float_to_u8(detail::half_to_float(values[1]));
				color.z = detail::float_to_u8(detail::half_to_float(values[2]));
				color.w = detail::float_to_u8(detail::half_to_float(values[3]));
				break;
			}
			case vk::Format::eR32Sfloat: {
				const auto pixel = (y * width + x) * 4;
				float value;
				std::memcpy(&value, m_Data.data() + pixel, sizeof(value));
				color.x = detail::float_to_u8(value);
				color.y = color.x;
				color.z = color.x;
				break;
			}
			case vk::Format::eR32G32B32A32Sfloat: {
				const auto pixel = (y * width + x) * 16;
				float values[4];
				std::memcpy(values, m_Data.data() + pixel, sizeof(values));
				color.x = detail::float_to_u8(values[0]);
				color.y = detail::float_to_u8(values[1]);
				color.z = detail::float_to_u8(values[2]);
				color.w = detail::float_to_u8(values[3]);
				break;
			}
			case vk::Format::eR8Srgb: {
				const auto pixel = y * width + x;
				color.x          = m_Data[pixel];
				color.y          = color.x;
				color.z          = color.x;
				break;
			}
			case vk::Format::eR8G8Srgb: {
				const auto pixel = (y * width + x) * 2;
				color.x          = m_Data[pixel + 0];
				color.y          = m_Data[pixel + 1];
				break;
			}
			case vk::Format::eR8G8B8Srgb: {
				const auto pixel = (y * width + x) * 3;
				color.x          = m_Data[pixel + 0];
				color.y          = m_Data[pixel + 1];
				color.z          = m_Data[pixel + 2];
				break;
			}
			case vk::Format::eR8G8B8A8Srgb: {
				const auto pixel = (y * width + x) * 4;
				color.x          = m_Data[pixel + 0];
				color.y          = m_Data[pixel + 1];
				color.z          = m_Data[pixel + 2];
				color.w          = m_Data[pixel + 3];
				break;
			}
			default:
				aby_rhi_assert(false, "unsupported vk::Format for read pixel: {}", vk::to_string(format));
				return { 0, 0, 0, 0 };
		}

		return color;
	}

	auto Texture::resize(uint32_t w, uint32_t h) -> void {
		if (bIsRenderTarget) {
			auto* r     = static_cast<vulkan::Renderer*>(Context::get().renderer());
			auto layout = m_Image.layout();
			m_Image.destroy();
			if (!m_Image.create(
			        vk::Extent3D(w, h, 1),
			        r->color_format(),
			        m_Samples,
			        vk::ImageUsageFlagBits::eTransferDst |
			            vk::ImageUsageFlagBits::eTransferSrc |
			            vk::ImageUsageFlagBits::eSampled |
			            vk::ImageUsageFlagBits::eColorAttachment)) {
				aby_rhi_err("failed to create image for texture: {}", m_ID);
			}
			r->immediate_submit([this, layout](vk::CommandBuffer cmd) {
				m_Image.transition(cmd, layout);
			});
			r->update_texture(m_ID, m_Image.view(), m_Sampler);
			m_Data.clear();
			m_FrameID = SIZE_MAX;
		} else {
			aby_rhi_assert("texture resizing not implemented for non render targets currently");
		}
	}

	auto Texture::format() -> vk::Format {
		return m_Image.format();
	}

	auto Texture::view() -> vk::ImageView {
		return m_Image.view();
	}

	auto Texture::id() -> uint32_t {
		return m_ID;
	}

	auto Texture::width() -> uint32_t {
		return m_Image.width();
	}

	auto Texture::height() -> uint32_t {
		return m_Image.height();
	}

	auto Texture::channels() -> uint32_t {
		return m_Channels;
	}

	auto Texture::data() -> uint8_t* {
		return m_Data.data();
	}

	auto Texture::bytes() -> size_t {
		return m_Data.size();
	}

	auto Texture::image() -> Image& {
		return m_Image;
	}

	auto Texture::is_render_target() const -> bool {
		return bIsRenderTarget;
	}

	auto Texture::load(const fs::path& path, uint32_t* out_width, uint32_t* out_height) -> bool {
		auto* io = Context::get().file_io();
		if (!io->read(path, &m_Data)) {
			aby_rhi_err("failed to read texture file: {}", path.string());
			return false;
		}

		int32_t w, h, c;
		stbi_uc* data = stbi_load_from_memory(m_Data.data(), m_Data.size(), &w, &h, &c, m_Channels);
		if (!data) {
			aby_rhi_err("failed to load texture from memory: {}. ({})", stbi_failure_reason(), path.string());
			return false;
		}

		*out_width  = w;
		*out_height = h;
		auto bytes  = w * h * m_Channels;

		m_Data.clear();
		m_Data.resize(bytes);
		m_Data.assign(data, data + bytes);

		aby_rhi_dbg("loaded texture: {} (w: {}, h: {}, c: {}, bytes: {})", path.string(), w, h, m_Channels, bytes);

		stbi_image_free(data);
		return true;
	}

} // namespace aby::rhi::vulkan
