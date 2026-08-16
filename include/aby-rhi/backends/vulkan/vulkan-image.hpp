#pragma once

#include "backends/vulkan/vulkan-buffer.hpp"
#include "backends/vulkan/vulkan-common.hpp"

#include <vk_mem_alloc.h>

namespace aby::rhi::vulkan {

	class Image {
	public:
		Image()             = default;
		Image(const Image&) = delete;
		Image(Image&& other) noexcept;

		auto create(vk::Extent3D extent, vk::Format format, vk::SampleCountFlagBits samples, vk::ImageUsageFlags usage, uint32_t mip_levels = 1) -> bool;
		auto wrap(vk::Image img, vk::ImageView view, vk::Extent3D extent, vk::Format format, vk::SampleCountFlagBits samples, vk::ImageUsageFlags usage) -> bool;
		auto destroy() -> void;

		auto transition(vk::CommandBuffer cmd, vk::ImageLayout dst_layout) -> void;
		auto transition(vk::CommandBuffer cmd, vk::ImageLayout dst_layout, uint32_t mip_level, uint32_t base_mip) -> void;
		auto copy_to(vk::CommandBuffer cmd, vk::Image dst, vk::Extent2D dst_sz) -> void;
		auto copy_to(vk::CommandBuffer cmd, Image& image) -> void;
		auto copy_to(vk::CommandBuffer cmd, uint32_t width, uint32_t height, uint32_t mip_level, vk::Filter filter_mode) -> void;
		auto copy_to(vk::CommandBuffer cmd, Buffer& buffer, uint32_t mip_level = 0) -> void;
		auto copy_from(vk::CommandBuffer cmd, Buffer& buffer, uint32_t mip_level = 0) -> void;

		auto img() -> vk::Image;
		auto view() -> vk::ImageView;
		auto width() const -> uint32_t;
		auto height() const -> uint32_t;
		auto depth() const -> uint32_t;
		auto extent() const -> const vk::Extent3D&;
		auto extent3d() const -> const vk::Extent3D&;
		auto extent2d() const -> vk::Extent2D;
		auto format() const -> vk::Format;
		auto layout(uint32_t mip_level = 0) const -> vk::ImageLayout;
		auto usage() const -> vk::ImageUsageFlags;
		auto samples() const -> vk::SampleCountFlagBits;
		auto mip_levels() const -> uint32_t;
		auto array_layers() const -> uint32_t;
		auto aspect() const -> vk::ImageAspectFlags;
		auto bpp() const -> uint8_t;

		auto is_multisampled() const -> bool;
		auto is_color() const -> bool;
		auto is_depth() const -> bool;

		explicit operator bool() const;

		operator vk::Image();
		operator VkImage();
		operator vk::ImageView();
		operator VkImageView();

		Image& operator=(const Image&) = delete;
		Image& operator=(Image&&) noexcept;
	private:
		vk::Image m_Img                        = VK_NULL_HANDLE;
		vk::ImageView m_View                   = VK_NULL_HANDLE;
		vk::Extent3D m_Extent                  = {};
		vk::Format m_Format                    = vk::Format::eUndefined;
		std::vector<vk::ImageLayout> m_Layouts = {};
		VmaAllocation m_Alloc                  = VK_NULL_HANDLE;
		vk::SampleCountFlagBits m_Samples      = vk::SampleCountFlagBits::e1;
		vk::ImageUsageFlags m_Usage            = {};
		uint32_t m_MipLevels                   = 1;
		uint32_t m_ArrayLayers                 = 1;
		vk::ImageAspectFlags m_Aspect          = vk::ImageAspectFlagBits::eColor;
		bool bOwnsImage                        = true;
	};

} // namespace aby::rhi::vulkan
