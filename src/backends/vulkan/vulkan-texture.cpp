#include "backends/vulkan/vulkan-texture.hpp"

#include "backends/vulkan/vulkan-common.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"

#include <stb_image/stb_image.h>
#include <stb_image/stb_image_resize2.h>
#include <stb_image/stb_image_write.h>

namespace aby::rhi::vulkan {

	Texture::Texture(ResourceID id, const fs::path& path, const TextureParams& params) :
	    m_ID(INVALID_ID),
	    m_Width(0),
	    m_Height(0),
	    m_Channels(0) {
		if (!load(path)) {
			return;
		}

		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		uint32_t max_mip_levels = 1 + static_cast<uint32_t>(std::floor(std::log2(std::max(m_Width, m_Height))));
		uint32_t mip_levels     = params.mip_levels == 0
		                            ? max_mip_levels
		                            : std::min(params.mip_levels, max_mip_levels);

		vk::Filter filter_mode;
		vk::SamplerMipmapMode sampler_mipmap_mode;
		switch (params.filtering) {
			case EFiltering::linear:
				filter_mode         = vk::Filter::eLinear;
				sampler_mipmap_mode = vk::SamplerMipmapMode::eLinear;
				break;
			case EFiltering::nearest:
				filter_mode         = vk::Filter::eNearest;
				sampler_mipmap_mode = vk::SamplerMipmapMode::eNearest;
				break;
			case EFiltering::cubic:
				filter_mode         = vk::Filter::eCubicIMG;
				sampler_mipmap_mode = vk::SamplerMipmapMode::eLinear;
				break;
			default:
				aby_rhi_assert(false, "unimplemented EFiltering mode in Vulkan backend: {}", params.filtering);
				break;
		}

		vk::SamplerAddressMode repeat_mode;
		switch (params.repeat_mode) {
			case ERepeatMode::repeat:
				repeat_mode = vk::SamplerAddressMode::eRepeat;
				break;
			case ERepeatMode::mirrored:
				repeat_mode = vk::SamplerAddressMode::eMirroredRepeat;
				break;
			case ERepeatMode::clamp_to_edge:
				repeat_mode = vk::SamplerAddressMode::eClampToEdge;
				break;
			case ERepeatMode::clamp_to_border:
				repeat_mode = vk::SamplerAddressMode::eClampToBorder;
				break;
			case ERepeatMode::mirrored_clamp_to_edge:
				repeat_mode = vk::SamplerAddressMode::eMirrorClampToEdge;
				break;
			default:
				aby_rhi_assert(false, "unimplemented ERepeatMode in Vulkan backend: {}", params.repeat_mode);
		}

		vk::Bool32 anisotropy_enable = (params.anisotropy_filtering == 0.f ? vk::False : vk::True);
		float max_aniostropy         = anisotropy_enable ? std::min<float>(params.anisotropy_filtering, r->max_sampler_anisotropy()) : 1.f;
		// uint32_t mip_levels = 1;

		vk::ImageCreateInfo image_create_info(
		    vk::ImageCreateFlags(0),
		    vk::ImageType::e2D,
		    vk::Format::eR8G8B8A8Srgb,
		    vk::Extent3D(m_Width, m_Height, 1),
		    mip_levels, /* mip levels */
		    1,          /* array layers*/
		    vk::SampleCountFlagBits::e1,
		    vk::ImageTiling::eOptimal,
		    vk::ImageUsageFlagBits::eTransferSrc |
		        vk::ImageUsageFlagBits::eTransferDst |
		        vk::ImageUsageFlagBits::eSampled);

		VmaAllocationCreateInfo alloc_create_info = {};
		alloc_create_info.usage                   = VMA_MEMORY_USAGE_GPU_ONLY;
		alloc_create_info.requiredFlags           = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

		vkassert(vmaCreateImage(
		             r->vma(),
		             image_create_info,
		             &alloc_create_info,
		             vkcast(m_Image),
		             &m_Alloc,
		             &m_AllocInfo),
		         "failed to create image for: {}", path.string());

		vk::ImageViewCreateInfo image_view_create_info(
		    vk::ImageViewCreateFlags(0),
		    m_Image,
		    vk::ImageViewType::e2D,
		    image_create_info.format,
		    vk::ComponentMapping{},
		    vk::ImageSubresourceRange(
		        vk::ImageAspectFlagBits::eColor,
		        0,          /* base mip level */
		        mip_levels, /* level count */
		        0,          /* base array layer */
		        1           /* layer count */
		        ));

		vkassert(vkCreateImageView(
		             r->device(),
		             vkcast(image_view_create_info),
		             allocator(),
		             vkcast(m_View)),
		         "failed to create image view for: {}", path.string());

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
			// Transition every mip level to TransferDst
			transition_image(
			    cmd,
			    m_Image,
			    vk::ImageLayout::eUndefined,
			    vk::ImageLayout::eTransferDstOptimal,
			    mip_levels);

			// Upload level 0
			vk::BufferImageCopy copy(
			    0,
			    0,
			    0,
			    vk::ImageSubresourceLayers(
			        vk::ImageAspectFlagBits::eColor,
			        0, // mip 0
			        0,
			        1),
			    vk::Offset3D(0, 0, 0),
			    image_create_info.extent);

			vkCmdCopyBufferToImage(
			    cmd,
			    staging,
			    m_Image,
			    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			    1,
			    vkcast(copy));

			int32_t mip_width  = m_Width;
			int32_t mip_height = m_Height;

			for (uint32_t i = 1; i < mip_levels; i++) {
				// Previous mip becomes transfer source
				transition_image(
				    cmd,
				    m_Image,
				    vk::ImageLayout::eTransferDstOptimal,
				    vk::ImageLayout::eTransferSrcOptimal,
				    1,
				    i - 1);

				vk::ImageBlit blit(
				    vk::ImageSubresourceLayers(
				        vk::ImageAspectFlagBits::eColor,
				        i - 1,
				        0,
				        1),
				    { vk::Offset3D(0, 0, 0),
				      vk::Offset3D(mip_width, mip_height, 1) },
				    vk::ImageSubresourceLayers(
				        vk::ImageAspectFlagBits::eColor,
				        i,
				        0,
				        1),
				    { vk::Offset3D(0, 0, 0),
				      vk::Offset3D(
				          std::max(1, mip_width / 2),
				          std::max(1, mip_height / 2),
				          1) });

				vkCmdBlitImage(
				    cmd,
				    m_Image,
				    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
				    m_Image,
				    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
				    1,
				    vkcast(blit),
				    static_cast<VkFilter>(filter_mode)); // we may want to just use linear anyways but for now we will respect the filter mode

				// Finished with previous mip
				transition_image(
				    cmd,
				    m_Image,
				    vk::ImageLayout::eTransferSrcOptimal,
				    vk::ImageLayout::eShaderReadOnlyOptimal,
				    1,
				    i - 1);

				mip_width  = std::max(1, mip_width / 2);
				mip_height = std::max(1, mip_height / 2);
			}

			// Final mip never became a source, so transition it separately.
			transition_image(
			    cmd,
			    m_Image,
			    vk::ImageLayout::eTransferDstOptimal,
			    vk::ImageLayout::eShaderReadOnlyOptimal,
			    1,
			    mip_levels - 1);

			return true;
		});

		m_ID = r->register_texture(id, m_View, m_Sampler);
	}

	Texture::~Texture() {
		destroy();
	}

	auto Texture::destroy() -> void {
		if (m_Image != VK_NULL_HANDLE) {
			auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
			std::memset(&m_AllocInfo, 0, sizeof(VmaAllocationInfo));
			vkDestroySampler(r->device(), m_Sampler, allocator());
			vkDestroyImageView(r->device(), m_View, allocator());
			vmaDestroyImage(r->vma(), m_Image, m_Alloc);
			m_Image   = VK_NULL_HANDLE;
			m_Alloc   = VK_NULL_HANDLE;
			m_Sampler = VK_NULL_HANDLE;
			m_View    = VK_NULL_HANDLE;
			m_Data.clear();
		}
	}

	auto Texture::id() -> uint32_t {
		return m_ID;
	}

	auto Texture::width() -> uint32_t {
		return m_Width;
	}

	auto Texture::height() -> uint32_t {
		return m_Height;
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

	auto Texture::load(const fs::path& path) -> bool {
		auto* io = Context::get().file_io();
		if (!io->read(path, &m_Data)) {
			aby_rhi_err("failed to read texture file: {}", path.string());
			return false;
		}

		int32_t w, h, c;
		stbi_uc* data = stbi_load_from_memory(m_Data.data(), m_Data.size(), &w, &h, &c, 4);
		if (!data) {
			aby_rhi_err("failed to load texture from memory: {}", stbi_failure_reason());
			return false;
		}

		m_Width    = w;
		m_Height   = h;
		m_Channels = 4;
		auto bytes = m_Width * m_Height * m_Channels;

		m_Data.clear();
		m_Data.resize(bytes);
		m_Data.assign(data, data + bytes);

		stbi_image_free(data);
		return true;
	}

} // namespace aby::rhi::vulkan
