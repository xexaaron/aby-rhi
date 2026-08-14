#include "backends/vulkan/vulkan-texture.hpp"

#include "backends/vulkan/vulkan-common.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"

#include <stb_image/stb_image.h>
#include <stb_image/stb_image_resize2.h>
#include <stb_image/stb_image_write.h>

namespace aby::rhi::vulkan {

	Texture::Texture(ResourceID id, const fs::path& path, const TextureParams& params) :
	    m_ID(INVALID_ID),
	    m_Channels(std::popcount(static_cast<uint8_t>(params.channels))),
	    m_Sampler(VK_NULL_HANDLE) {
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
		        vk::SampleCountFlagBits::e1,
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
			m_Image.copy_to(cmd, staging, 0);

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
	    m_Sampler(VK_NULL_HANDLE) {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		if (!m_Image.create(
		        vk::Extent3D(width, height, 1),
		        r->color_format(),
		        samples,
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
