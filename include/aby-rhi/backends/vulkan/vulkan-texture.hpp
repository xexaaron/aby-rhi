#pragma once
#include "backends/vulkan/vulkan-buffer.hpp"
#include "backends/vulkan/vulkan-image.hpp"
#include "context.hpp"
#include "texture.hpp"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

	class Texture : public rhi::Texture {
	public:
		Texture(ResourceID id, const fs::path& path, const TextureParams& params);
		Texture(ResourceID id, uint32_t width, uint32_t height, uint8_t channels, vk::SampleCountFlagBits samples = vk::SampleCountFlagBits::e1);
		~Texture();

		auto destroy() -> void override;

		auto format() -> vk::Format;
		auto view() -> vk::ImageView;
		auto id() -> uint32_t override;
		auto width() -> uint32_t override;
		auto height() -> uint32_t override;
		auto channels() -> uint32_t override;
		auto data() -> uint8_t* override;
		auto bytes() -> size_t override;
		auto image() -> Image&;
	private:
		auto load(const fs::path& path, uint32_t* out_width, uint32_t* out_height) -> bool;
	private:
		uint32_t m_ID;
		uint32_t m_Channels;
		vk::Sampler m_Sampler;
		Image m_Image;
		std::vector<uint8_t> m_Data;
	};

} // namespace aby::rhi::vulkan
