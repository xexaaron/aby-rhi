#include "backends/vulkan/vulkan-image.hpp"

#include "backends/vulkan/vulkan-renderer.hpp"
#include "context.hpp"

namespace aby::rhi::vulkan {

	Image::Image(Image&& other) noexcept {
		*this = std::move(other);
	}

	auto Image::create(vk::Extent3D extent, vk::Format format, vk::SampleCountFlagBits samples, vk::ImageUsageFlags usage) -> bool {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		m_Extent      = extent;
		m_Format      = format;
		m_Samples     = samples;
		m_Usage       = usage;
		m_MipLevels   = 1;
		m_ArrayLayers = 1;
		bOwnsImage    = true;

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
		m_Img         = img;
		m_View        = view;
		m_Extent      = extent;
		m_Format      = format;
		m_Layout      = vk::ImageLayout::eUndefined;
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
			m_Img         = VK_NULL_HANDLE;
			m_View        = VK_NULL_HANDLE;
			m_Extent      = vk::Extent3D(0, 0, 1);
			m_Format      = vk::Format::eUndefined;
			m_Layout      = vk::ImageLayout::eUndefined;
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
		vk::ImageMemoryBarrier2 image_barrier(
		    vk::PipelineStageFlagBits2::eAllCommands,
		    vk::AccessFlagBits2::eMemoryWrite,
		    vk::PipelineStageFlagBits2::eAllCommands,
		    vk::AccessFlagBits2::eMemoryWrite | vk::AccessFlagBits2::eMemoryRead,
		    m_Layout,
		    dst_layout,
		    0, /* src queue */
		    0, /* dst queue */
		    m_Img,
		    vk::ImageSubresourceRange(
		        m_Aspect,
		        0,
		        m_MipLevels,
		        0,
		        m_ArrayLayers));

		vk::DependencyInfo dep_info({}, 0, nullptr, 0, nullptr, 1, &image_barrier);
		vkCmdPipelineBarrier2(cmd, vkcast(dep_info));

		m_Layout = dst_layout;
	}

	auto Image::copy_to(vk::CommandBuffer cmd, vk::Image dst, vk::Extent2D dst_sz) -> void {
		aby_rhi_assert(m_Layout == vk::ImageLayout::eTransferSrcOptimal, "src image was not transitioned before copy");

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
		aby_rhi_assert(m_Layout == vk::ImageLayout::eTransferSrcOptimal, "src image was not transitioned before copy");
		aby_rhi_assert(image.m_Layout == vk::ImageLayout::eTransferDstOptimal, "dst image was not transitioned before copy");

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

	auto Image::layout() const -> vk::ImageLayout {
		return m_Layout;
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
			m_Img         = std::exchange(other.m_Img, VK_NULL_HANDLE);
			m_View        = std::exchange(other.m_View, VK_NULL_HANDLE);
			m_Extent      = other.m_Extent;
			m_Format      = other.m_Format;
			m_Layout      = other.m_Layout;
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
