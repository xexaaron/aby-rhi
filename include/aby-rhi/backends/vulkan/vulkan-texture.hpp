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

		auto sync() -> void override;
		auto read_px(uint32_t x, uint32_t y) -> vec4<uint8_t> override;
		auto resize(uint32_t w, uint32_t h) -> void override;

		auto format() -> vk::Format;
		auto view() -> vk::ImageView;
		auto id() -> uint32_t override;
		auto width() -> uint32_t override;
		auto height() -> uint32_t override;
		auto channels() -> uint32_t override;
		auto data() -> uint8_t* override;
		auto bytes() -> size_t override;
		auto image() -> Image&;
		auto is_render_target() const -> bool;
	private:
		auto load(const fs::path& path, uint32_t* out_width, uint32_t* out_height) -> bool;
	private:
		uint32_t m_ID;
		uint32_t m_Channels;
		vk::SampleCountFlagBits m_Samples;
		vk::Sampler m_Sampler;
		Image m_Image;
		const bool bIsRenderTarget;
		size_t m_FrameID;
		std::vector<uint8_t> m_Data;
	};

} // namespace aby::rhi::vulkan
