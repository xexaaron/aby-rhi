#include "backends/vulkan/vulkan-image.hpp"

#include "backends/vulkan/vulkan-renderer.hpp"
#include "context.hpp"

#include <algorithm>
namespace aby::rhi::vulkan {

	Image::Image(Image&& other) noexcept {
		*this = std::move(other);
	}

	auto Image::create(vk::Extent3D extent, vk::Format format, vk::SampleCountFlagBits samples, vk::ImageUsageFlags usage, uint32_t mip_levels) -> bool {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		m_Extent      = extent;
		m_Format      = format;
		m_Samples     = samples;
		m_Usage       = usage;
		m_MipLevels   = mip_levels;
		m_ArrayLayers = 1;
		bOwnsImage    = true;
		m_Layouts.resize(mip_levels, vk::ImageLayout::eUndefined);

		vk::ImageCreateInfo image_create_info(
		    vk::ImageCreateFlags(0),
		    vk::ImageType::e2D,
		    m_Format,
		    m_Extent,
		    m_MipLevels,
		    m_ArrayLayers,
		    m_Samples,
		    vk::ImageTiling::eOptimal,
		    m_Usage);
		VmaAllocationCreateInfo image_alloc_info = {};
		image_alloc_info.usage                   = VMA_MEMORY_USAGE_GPU_ONLY;
		image_alloc_info.requiredFlags           = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
		vkcheck(vmaCreateImage(
		            r->vma(),
		            vkcast(image_create_info),
		            &image_alloc_info,
		            vkcast(m_Img),
		            &m_Alloc,
		            nullptr),
		        "failed to create draw image image (using VMA)");

		vk::ImageViewCreateInfo image_view_create_info(
		    vk::ImageViewCreateFlags(0),
		    m_Img,
		    vk::ImageViewType::e2D,
		    m_Format,
		    vk::ComponentMapping{},
		    vk::ImageSubresourceRange(
		        m_Aspect,
		        0, /* base mip level */
		        m_MipLevels,
		        0,
		        m_ArrayLayers /* layer count */
		        ));

		vkcheck(vkCreateImageView(
		            r->device(),
		            vkcast(image_view_create_info),
		            allocator(),
		            vkcast(m_View)),
		        "failed to create draw image view");

		return true;
	}

	auto Image::wrap(vk::Image img, vk::ImageView view, vk::Extent3D extent, vk::Format format, vk::SampleCountFlagBits samples, vk::ImageUsageFlags usage) -> bool {
		m_Img    = img;
		m_View   = view;
		m_Extent = extent;
		m_Format = format;
		m_Layouts.assign(1, vk::ImageLayout::eUndefined);
		m_Alloc       = VK_NULL_HANDLE;
		m_Samples     = samples;
		m_Usage       = usage;
		m_MipLevels   = 1;
		m_ArrayLayers = 1;
		m_Aspect      = vk::ImageAspectFlagBits::eColor;
		bOwnsImage    = false;
		return m_View != VK_NULL_HANDLE && m_Img != VK_NULL_HANDLE;
	}

	auto Image::destroy() -> void {
		if (m_Img && bOwnsImage) {
			auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
			vkDestroyImageView(r->device(), m_View, allocator());
			vmaDestroyImage(r->vma(), m_Img, m_Alloc);
			m_Img    = VK_NULL_HANDLE;
			m_View   = VK_NULL_HANDLE;
			m_Extent = vk::Extent3D(0, 0, 1);
			m_Format = vk::Format::eUndefined;
			m_Layouts.clear();
			m_Alloc       = VK_NULL_HANDLE;
			m_Samples     = vk::SampleCountFlagBits::e1;
			m_Usage       = {};
			m_MipLevels   = 1;
			m_ArrayLayers = 1;
			m_Aspect      = vk::ImageAspectFlagBits::eColor;
		} else if (m_View) {
			auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
			vkDestroyImageView(r->device(), m_View, allocator());
		}
	}

	auto Image::transition(vk::CommandBuffer cmd, vk::ImageLayout dst_layout) -> void {
		transition(cmd, dst_layout, m_MipLevels, 0);
	}

	auto Image::transition(vk::CommandBuffer cmd, vk::ImageLayout dst_layout, uint32_t mip_levels, uint32_t base_mip) -> void {
		aby_rhi_assert(base_mip + mip_levels <= m_MipLevels, "image mip transition range exceeds mip levels");

		for (uint32_t i = 0; i < mip_levels; ++i) {
			auto mip = base_mip + i;

			vk::ImageMemoryBarrier2 image_barrier(
			    vk::PipelineStageFlagBits2::eAllCommands,
			    vk::AccessFlagBits2::eMemoryWrite,
			    vk::PipelineStageFlagBits2::eAllCommands,
			    vk::AccessFlagBits2::eMemoryWrite |
			        vk::AccessFlagBits2::eMemoryRead,
			    m_Layouts[mip],
			    dst_layout,
			    0,
			    0,
			    m_Img,
			    vk::ImageSubresourceRange(
			        m_Aspect,
			        mip,
			        1,
			        0,
			        m_ArrayLayers));

			vk::DependencyInfo dep_info(
			    {},
			    0, nullptr,
			    0, nullptr,
			    1, &image_barrier);

			vkCmdPipelineBarrier2(cmd, vkcast(dep_info));

			m_Layouts[mip] = dst_layout;
		}
	}

	auto Image::copy_to(vk::CommandBuffer cmd, vk::Image dst, vk::Extent2D dst_sz) -> void {
		aby_rhi_assert(m_Layouts[0] == vk::ImageLayout::eTransferSrcOptimal, "src image was not transitioned before copy");

		VkImageBlit2 blit_region{ .sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2, .pNext = nullptr };

		blit_region.srcOffsets[1].x = m_Extent.width;
		blit_region.srcOffsets[1].y = m_Extent.height;
		blit_region.srcOffsets[1].z = 1;

		blit_region.dstOffsets[1].x = dst_sz.width;
		blit_region.dstOffsets[1].y = dst_sz.height;
		blit_region.dstOffsets[1].z = 1;

		blit_region.srcSubresource.aspectMask     = static_cast<VkImageAspectFlags>(m_Aspect);
		blit_region.srcSubresource.baseArrayLayer = 0;
		blit_region.srcSubresource.layerCount     = 1;
		blit_region.srcSubresource.mipLevel       = 0;
		blit_region.dstSubresource.aspectMask     = static_cast<VkImageAspectFlags>(m_Aspect);
		blit_region.dstSubresource.baseArrayLayer = 0;
		blit_region.dstSubresource.layerCount     = 1;
		blit_region.dstSubresource.mipLevel       = 0;

		VkBlitImageInfo2 blit_info{ .sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2, .pNext = nullptr };
		blit_info.dstImage       = dst;
		blit_info.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		blit_info.srcImage       = m_Img;
		blit_info.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		blit_info.filter         = VK_FILTER_LINEAR;
		blit_info.regionCount    = 1;
		blit_info.pRegions       = &blit_region;

		vkCmdBlitImage2(cmd, &blit_info);
	}

	auto Image::copy_to(vk::CommandBuffer cmd, Image& image) -> void {
		aby_rhi_assert(m_Layouts[0] == vk::ImageLayout::eTransferSrcOptimal, "src image was not transitioned before copy");
		aby_rhi_assert(image.m_Layouts[0] == vk::ImageLayout::eTransferDstOptimal, "dst image was not transitioned before copy");

		VkImageBlit2 blit_region{ .sType = VK_STRUCTURE_TYPE_IMAGE_BLIT_2, .pNext = nullptr };

		blit_region.srcOffsets[1].x = m_Extent.width;
		blit_region.srcOffsets[1].y = m_Extent.height;
		blit_region.srcOffsets[1].z = m_Extent.depth;

		blit_region.dstOffsets[1].x = image.m_Extent.width;
		blit_region.dstOffsets[1].y = image.m_Extent.height;
		blit_region.dstOffsets[1].z = image.m_Extent.depth;

		blit_region.srcSubresource.aspectMask     = static_cast<VkImageAspectFlags>(m_Aspect);
		blit_region.srcSubresource.baseArrayLayer = 0;
		blit_region.srcSubresource.layerCount     = 1;
		blit_region.srcSubresource.mipLevel       = 0;
		blit_region.dstSubresource.aspectMask     = static_cast<VkImageAspectFlags>(image.m_Aspect);
		blit_region.dstSubresource.baseArrayLayer = 0;
		blit_region.dstSubresource.layerCount     = 1;
		blit_region.dstSubresource.mipLevel       = 0;

		VkBlitImageInfo2 blit_info{ .sType = VK_STRUCTURE_TYPE_BLIT_IMAGE_INFO_2, .pNext = nullptr };
		blit_info.dstImage       = image.m_Img;
		blit_info.dstImageLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
		blit_info.srcImage       = m_Img;
		blit_info.srcImageLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
		blit_info.filter         = VK_FILTER_LINEAR;
		blit_info.regionCount    = 1;
		blit_info.pRegions       = &blit_region;
		vkCmdBlitImage2(cmd, &blit_info);
	}

	auto Image::copy_to(vk::CommandBuffer cmd, Buffer& buffer, uint32_t mip_level) -> void {
		aby_rhi_assert(m_Layouts[mip_level] == vk::ImageLayout::eTransferSrcOptimal, "destination mip was not transitioned to transfer src");
		auto mip_extent = vk::Extent3D{
			std::max(1u, m_Extent.width >> mip_level),
			std::max(1u, m_Extent.height >> mip_level),
			std::max(1u, m_Extent.depth >> mip_level)
		};

		vk::BufferImageCopy copy(
		    0,
		    0,
		    0,
		    vk::ImageSubresourceLayers(
		        vk::ImageAspectFlagBits::eColor,
		        mip_level, // mip 0
		        0,
		        1),
		    vk::Offset3D(0, 0, 0),
		    mip_extent);

		vkCmdCopyImageToBuffer(
			cmd,
			m_Img,
			static_cast<VkImageLayout>(m_Layouts[mip_level]),
			buffer,
			1,
			vkcast(copy));
	}

	auto Image::copy_from(vk::CommandBuffer cmd, Buffer& buffer, uint32_t mip_level) -> void {
		aby_rhi_assert(m_Layouts[mip_level] == vk::ImageLayout::eTransferDstOptimal, "destination mip was not transitioned to transfer dst");

		auto mip_extent = vk::Extent3D{
			std::max(1u, m_Extent.width >> mip_level),
			std::max(1u, m_Extent.height >> mip_level),
			std::max(1u, m_Extent.depth >> mip_level)
		};

		vk::BufferImageCopy copy(
		    0,
		    0,
		    0,
		    vk::ImageSubresourceLayers(
		        vk::ImageAspectFlagBits::eColor,
		        mip_level, // mip 0
		        0,
		        1),
		    vk::Offset3D(0, 0, 0),
		    mip_extent);

		vkCmdCopyBufferToImage(
		    cmd,
		    buffer,
		    m_Img,
		    static_cast<VkImageLayout>(m_Layouts[mip_level]),
		    1,
		    vkcast(copy));
	}

	auto Image::copy_to(vk::CommandBuffer cmd, uint32_t width, uint32_t height, uint32_t mip_level, vk::Filter filter_mode) -> void {
		aby_rhi_assert(mip_level > 0, "mip level must be greater than zero when generating mipmaps");
		aby_rhi_assert(m_Layouts[mip_level - 1] == vk::ImageLayout::eTransferSrcOptimal, "source mip was not transitioned to transfer source");
		aby_rhi_assert(m_Layouts[mip_level] == vk::ImageLayout::eTransferDstOptimal, "destination mip was not transitioned to transfer destination");

		vk::ImageBlit blit(
		    vk::ImageSubresourceLayers(
		        vk::ImageAspectFlagBits::eColor,
		        mip_level - 1,
		        0,
		        1),
		    { vk::Offset3D(0, 0, 0),
		      vk::Offset3D(width, height, 1) },
		    vk::ImageSubresourceLayers(
		        vk::ImageAspectFlagBits::eColor,
		        mip_level,
		        0,
		        1),
		    { vk::Offset3D(0, 0, 0),
		      vk::Offset3D(
		          std::max<int32_t>(1, width / 2),
		          std::max<int32_t>(1, height / 2),
		          1) });

		vkCmdBlitImage(
		    cmd,
		    m_Img,
		    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
		    m_Img,
		    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
		    1,
		    vkcast(blit),
		    static_cast<VkFilter>(filter_mode));
	}

	auto Image::img() -> vk::Image {
		return m_Img;
	}

	auto Image::view() -> vk::ImageView {
		return m_View;
	}

	auto Image::width() const -> uint32_t {
		return m_Extent.width;
	}

	auto Image::height() const -> uint32_t {
		return m_Extent.height;
	}

	auto Image::depth() const -> uint32_t {
		return m_Extent.depth;
	}

	auto Image::extent() const -> const vk::Extent3D& {
		return m_Extent;
	}

	auto Image::extent3d() const -> const vk::Extent3D& {
		return m_Extent;
	}

	auto Image::extent2d() const -> vk::Extent2D {
		return vk::Extent2D(m_Extent.width, m_Extent.height);
	}

	auto Image::format() const -> vk::Format {
		return m_Format;
	}

	auto Image::layout(uint32_t mip_level) const -> vk::ImageLayout {
		return m_Layouts[mip_level];
	}

	auto Image::usage() const -> vk::ImageUsageFlags {
		return m_Usage;
	}

	auto Image::samples() const -> vk::SampleCountFlagBits {
		return m_Samples;
	}

	auto Image::mip_levels() const -> uint32_t {
		return m_MipLevels;
	}

	auto Image::array_layers() const -> uint32_t {
		return m_ArrayLayers;
	}

	auto Image::aspect() const -> vk::ImageAspectFlags {
		return m_Aspect;
	}

	auto Image::bpp() const -> uint8_t {
		switch (m_Format) {
			// 8-bit
			case vk::Format::eR8Unorm:
			case vk::Format::eR8Snorm:
			case vk::Format::eR8Uint:
			case vk::Format::eR8Sint:
			case vk::Format::eR8Srgb:
				return 1;

			// 16-bit
			case vk::Format::eR8G8Unorm:
			case vk::Format::eR8G8Snorm:
			case vk::Format::eR8G8Uint:
			case vk::Format::eR8G8Sint:
			case vk::Format::eR8G8Srgb:
				return 2;

			case vk::Format::eR16Unorm:
			case vk::Format::eR16Snorm:
			case vk::Format::eR16Uint:
			case vk::Format::eR16Sint:
			case vk::Format::eR16Sfloat:
				return 2;

			// 24-bit
			case vk::Format::eR8G8B8Unorm:
			case vk::Format::eR8G8B8Srgb:
			case vk::Format::eR8G8B8Snorm:
			case vk::Format::eR8G8B8Uint:
			case vk::Format::eR8G8B8Sint:
				return 3;

			// 32-bit
			case vk::Format::eR8G8B8A8Unorm:
			case vk::Format::eR8G8B8A8Srgb:
			case vk::Format::eR8G8B8A8Snorm:
			case vk::Format::eR8G8B8A8Uint:
			case vk::Format::eR8G8B8A8Sint:

			case vk::Format::eB8G8R8A8Unorm:
			case vk::Format::eB8G8R8A8Srgb:

			case vk::Format::eR16G16Unorm:
			case vk::Format::eR16G16Snorm:
			case vk::Format::eR16G16Uint:
			case vk::Format::eR16G16Sint:
			case vk::Format::eR16G16Sfloat:

			case vk::Format::eR32Uint:
			case vk::Format::eR32Sint:
			case vk::Format::eR32Sfloat:
				return 4;

			// 64-bit
			case vk::Format::eR16G16B16A16Sfloat:
			case vk::Format::eR16G16B16A16Uint:
			case vk::Format::eR16G16B16A16Sint:
				return 8;

			// 128-bit
			case vk::Format::eR32G32B32A32Sfloat:
			case vk::Format::eR32G32B32A32Uint:
			case vk::Format::eR32G32B32A32Sint:
				return 16;

			default:
				aby_rhi_assert(false,
				               "unsupported Vulkan format for bpp calculation: {}",
				               vk::to_string(m_Format));

				return 0;
		}
		return 0;
	}

	auto Image::is_multisampled() const -> bool {
		return m_Samples != vk::SampleCountFlagBits::e1;
	}

	auto Image::is_color() const -> bool {
		return (m_Aspect & vk::ImageAspectFlagBits::eColor) != vk::ImageAspectFlagBits::eNone;
	}

	auto Image::is_depth() const -> bool {
		return (m_Aspect & vk::ImageAspectFlagBits::eDepth) != vk::ImageAspectFlagBits::eNone;
	}

	Image& Image::operator=(Image&& other) noexcept {
		if (this != &other) {
			m_Img     = std::exchange(other.m_Img, VK_NULL_HANDLE);
			m_View    = std::exchange(other.m_View, VK_NULL_HANDLE);
			m_Extent  = other.m_Extent;
			m_Format  = other.m_Format;
			m_Layouts = other.m_Layouts;
			other.m_Layouts.clear();
			m_Alloc       = std::exchange(other.m_Alloc, VK_NULL_HANDLE);
			m_Samples     = other.m_Samples;
			m_Usage       = other.m_Usage;
			m_MipLevels   = other.m_MipLevels;
			m_ArrayLayers = other.m_ArrayLayers;
			m_Aspect      = other.m_Aspect;
			bOwnsImage    = other.bOwnsImage;
		}

		return *this;
	}

	Image::operator vk::Image() {
		return m_Img;
	}

	Image::operator VkImage() {
		return m_Img;
	}

	Image::operator vk::ImageView() {
		return m_View;
	}

	Image::operator VkImageView() {
		return m_View;
	}

	Image::operator bool() const {
		if (bOwnsImage)
			return m_Img != VK_NULL_HANDLE;
		else
			return m_View != VK_NULL_HANDLE;
	}

} // namespace aby::rhi::vulkan
