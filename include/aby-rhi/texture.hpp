#pragma once
#include "common.hpp"
#include "resource.hpp"

#include <filesystem>

namespace aby::rhi {

	namespace fs = std::filesystem;

	struct TextureParams {
		auto set_mip_levels(uint32_t mip_levels) -> TextureParams&;
		auto set_anisotropy_filtering(float anisotropy) -> TextureParams&;
		auto set_filtering(EFiltering filtering) -> TextureParams&;
		auto set_repeat_mode(ERepeatMode repeat_mode) -> TextureParams&;
		auto set_texture_usage(ETextureUsage texture_usage) -> TextureParams&;
		auto set_channels(EChannels channels) -> TextureParams&;

		uint32_t mip_levels         = 0;   // if 0 then generate the mips based on the textures size. if 1 then no mips.
		float anisotropy_filtering  = 0.f; // if 0, then disabled, otherwise clamped between the value and the devices maximum supported.
		EFiltering filtering        = EFiltering::linear;
		ERepeatMode repeat_mode     = ERepeatMode::repeat;
		ETextureUsage texture_usage = ETextureUsage::albedo; // color space of texture
		EChannels channels          = EChannels::rgba;
	};

	class Texture {
	public:
		static constexpr uint32_t INVALID_ID = UINT32_MAX;
	public:
		static auto create(const fs::path& rel_path, const TextureParams& params = {}) -> ResourcePtr<Texture, EResource::texture>;
		static auto create_render_target(uint8_t channels, EAntiAliasing aliasing = EAntiAliasing::none) -> ResourcePtr<Texture, EResource::texture>;
		virtual ~Texture() = default;

		virtual auto destroy() -> void = 0;

		/**
		 * @brief Synchronizes the texture's GPU data with its CPU buffer
		 * 		  For render targets this performs a GPU to CPU readback. For textures
		 * 		  created from a file it will read directly from the cpu buffer
		 * @note For render targets this must be called once per frame before calling read_px() or read_pxs()
		 * 		 if the current frames pixels are required
		 */
		virtual auto sync() -> void                                   = 0;
		/**
		 * @brief Read a pixel from the texture's CPU side pixel buffer.
		 * 		  For render targets sync() must have been called for the current frame before
		 * 		  calling this function. For textures created from a file it will read directly		
		 * 		  from the cpu side buffer.
		 * @param x The x position.
		 * @param y The y position.
		 * @return CPU side rgba8 pixel data.
		 */
		virtual auto read_px(uint32_t x, uint32_t y) -> vec4<uint8_t> = 0;

		/** @brief Read a contiguous range of pixels from the texture's CPU side pixel buffer
		 * 		  For render targets sync() must have been called for the current frame before
		 * 		  calling this function. For textures created from a file it will read directly		
		 * 		  from the cpu side buffer.
		 * @param x0 The x start position.
		 * @param y0 The y start position.
		 * @param x1 the x end position.
		 * @param y1 the y end position.
		 * @return CPU side rgba8 pixel data.
		 */
		virtual auto read_pxs(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) -> std::vector<vec4<uint8_t>> = 0;

		/**
		 * @brief Resize the texture
		 * @param w new width
		 * @param h new height
		 */
		virtual auto resize(uint32_t w, uint32_t h) -> void  = 0;
		/**
		 * @brief Write the image to a file.
		 * @param rel_path Output image type determined by path extension.
		 */
		virtual auto write(const fs::path& rel_path) -> void = 0;
		/**
		 * @brief An id to the texture within an array or descriptor set that 
		 * 		  can be sent to the GPU to access bindless textures
		 */
		virtual auto id() -> uint32_t                        = 0;
		virtual auto width() -> uint32_t                     = 0;
		virtual auto height() -> uint32_t                    = 0;
		virtual auto channels() -> uint32_t                  = 0;
		virtual auto data() -> uint8_t*                      = 0;
		virtual auto bytes() -> size_t                       = 0;
	};

	using TexturePtr = ResourcePtr<Texture, EResource::texture>;

} // namespace aby::rhi
