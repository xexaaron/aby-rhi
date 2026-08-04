#include "backends/vulkan/vulkan-texture.hpp"

#include "backends/vulkan/vulkan-common.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"

#include <stb_image/stb_image.h>
#include <stb_image/stb_image_resize2.h>
#include <stb_image/stb_image_write.h>

namespace aby::rhi::vulkan {

	Texture::Texture(const fs::path& path) :
	    m_ID(INVALID_ID),
	    m_Width(0),
	    m_Height(0),
	    m_Channels(0) {
		if (!load(path)) {
			return;
		}

		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		vk::ImageCreateInfo image_create_info(
		    vk::ImageCreateFlags(0),
		    vk::ImageType::e2D,
		    vk::Format::eR8G8B8A8Srgb,
		    vk::Extent3D(m_Width, m_Height, 1),
		    1, /* mip levels */
		    1, /* array layers*/
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
		        0, /* base mip level */
		        1, /* level count */
		        0, /* base array layer */
		        1  /* layer count */
		        ));

		vkassert(vkCreateImageView(
		             r->device(),
		             vkcast(image_view_create_info),
		             allocator(),
		             vkcast(m_View)),
		         "failed to create image view for: {}", path.string());

		auto uvw_address_mode = vk::SamplerAddressMode::eRepeat;
		vk::SamplerCreateInfo sampler_create_info(
		    vk::SamplerCreateFlags(),
		    vk::Filter::eNearest,                    /* mag filter */
		    vk::Filter::eNearest,                    /* min filter */
		    vk::SamplerMipmapMode::eNearest,         /* mipmap mode */
		    uvw_address_mode,                        /* u */
		    uvw_address_mode,                        /* v */
		    uvw_address_mode,                        /* w */
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
		         "failed to create texture sampler for: {}", path.string());

		auto staging = Buffer(
		    this->bytes(),
		    vk::BufferUsageFlagBits::eTransferSrc,
		    VMA_MEMORY_USAGE_CPU_ONLY);

		staging.write(m_Data.data(), m_Data.size());

		r->immediate_submit([&](vk::CommandBuffer cmd) {
			transition_image(
			    cmd,
			    m_Image,
			    vk::ImageLayout::eUndefined,
			    vk::ImageLayout::eTransferDstOptimal);

			vk::BufferImageCopy copy(
			    0, // bufferOffset
			    0, // bufferRowLength (tightly packed)
			    0, // bufferImageHeight
			    vk::ImageSubresourceLayers(
			        vk::ImageAspectFlagBits::eColor,
			        0,
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

			transition_image(
			    cmd,
			    m_Image,
			    vk::ImageLayout::eTransferDstOptimal,
			    vk::ImageLayout::eShaderReadOnlyOptimal);

			return true;
		});

		m_ID = r->register_texture(m_View, m_Sampler);
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
