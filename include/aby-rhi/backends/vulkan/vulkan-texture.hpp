#pragma once
#include "backends/vulkan/vulkan-buffer.hpp"
#include "context.hpp"
#include "texture.hpp"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

	class Texture : public rhi::Texture {
	public:
		Texture(const fs::path& path);
		~Texture();
		
		auto destroy() -> void override;
		auto id() -> uint32_t override;
		auto width() -> uint32_t override;
		auto height() -> uint32_t override;
		auto channels() -> uint32_t override;
		auto data() -> uint8_t* override;
		auto bytes() -> size_t override;
	private:
		auto load(const fs::path& path) -> bool;
	private:
		uint32_t m_ID;
		uint32_t m_Width;
		uint32_t m_Height;
		uint32_t m_Channels;

		VmaAllocationInfo m_AllocInfo;
		VmaAllocation m_Alloc;
		vk::Image m_Image;
		vk::ImageView m_View;
		vk::Sampler m_Sampler;

		std::vector<uint8_t> m_Data;
	};

} // namespace aby::rhi::vulkan
