#pragma once
#include "common.hpp"
#include "resource.hpp"

#include <filesystem>

namespace aby::rhi {

	namespace fs = std::filesystem;

	struct ABY_RHI_API TextureParams {
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

	class ABY_RHI_API Texture {
	public:
		static constexpr uint32_t INVALID_ID = UINT32_MAX;
	public:
		/**
		* @brief Create a resource ptr to a texture
		* @param[in] rel_path a path relative to the CWD
		* @param[in] params the texture parameter configuration
		* @return ResourcePtr<Texture, EResource::texture>
		*/
		static auto create(const fs::path& rel_path, const TextureParams& params = {}) -> ResourcePtr<Texture, EResource::texture>;
		/**
		* @brief Create a resource ptr to a texture 
		* @param[in] width the width of the texture bytes
		* @param[in] height the height of the texture bytes
		* @param[in] channels the channels of the texture bytes
		* @return ResourcePtr<Texture, EResource::texture>
		*/
		static auto create(uint32_t width, uint32_t height, uint8_t channels, std::vector<uint8_t>&& bytes) -> ResourcePtr<Texture, EResource::texture>;
		/**
		* @brief Create a resource ptr to a texture used for render targets
		* @param[in] channels the desired channel count
		* @param[in] aliasing the msaa antialiasing. The render pass must have the same anti-aliasing as this render target
		* @return ResourcePtr<Texture, EResource::texture>
		*/
		static auto create_render_target(uint8_t channels, EAntiAliasing aliasing = EAntiAliasing::none) -> ResourcePtr<Texture, EResource::texture>;
		virtual ~Texture()                                                                                      = default;
		/**
		* @brief Destroy the texture and the backend data
		*/
		virtual auto destroy() -> void                                                                          = 0;
		/**
		 * @brief Synchronizes the texture's GPU data with its CPU buffer
		 * 		  For render targets this performs a GPU to CPU readback. For textures
		 * 		  created from a file it will read directly from the cpu buffer
		 * @note For render targets this must be called once per frame before calling @c read_px() or @c read_pxs()
		 * 		 if the current frames pixels are required
		 */
		virtual auto sync() -> void                                                                             = 0;
		/**
		 * @brief Read a pixel from the texture's CPU side pixel buffer.
		 * 		  For render targets @c sync() must have been called for the current frame before
		 * 		  calling this function. For textures created from a file it will read directly		
		 * 		  from the cpu side buffer.
		 * @param[in] x The x position.
		 * @param[in] y The y position.
		 * @return CPU side rgba8 pixel data.
		 */
		virtual auto read_px(uint32_t x, uint32_t y) -> vec4<uint8_t>                                           = 0;
		/** @brief Read a contiguous range of pixels from the texture's CPU side pixel buffer
		 * 		  For render targets @c sync() must have been called for the current frame before
		 * 		  calling this function. For textures created from a file it will read directly		
		 * 		  from the cpu side buffer.
		 * @param[in] x0 The x start position.
		 * @param[in] y0 The y start position.
		 * @param[in] x1 the x end position.
		 * @param[in] y1 the y end position.
		 * @return CPU side rgba8 pixel data.
		 */
		virtual auto read_pxs(uint32_t x0, uint32_t y0, uint32_t x1, uint32_t y1) -> std::vector<vec4<uint8_t>> = 0;
		/**
		 * @brief Resize the texture
		 * @param[in] w new width
		 * @param[in] h new height
		 */
		virtual auto resize(uint32_t w, uint32_t h) -> void                                                     = 0;
		/**
		 * @brief Write the image to a file.
		 * @param rel_path path relative to the CWD.
		 * @param jpeg_quality a value between 1-100 (90 is a good middleground for high quality and reasonable file size)
		 * @note The output image is determined by the file extension: [.bmp, .hdr, .jpeg, .png, .tga]
		 */
		virtual auto write(const fs::path& rel_path, int jpeg_quality = 90) -> void                             = 0;
		/**
		 * @brief An id to the texture within an array or descriptor set that 
		 * 		  can be sent to the GPU to access bindless textures
		 */
		virtual auto id() -> uint32_t                                                                           = 0;
		/**
		* @brief Get the texture width
		*/
		virtual auto width() -> uint32_t                                                                        = 0;
		/**
		* @brief Get the texture height
		*/
		virtual auto height() -> uint32_t                                                                       = 0;
		/**
		* @brief Get the texture channel count
		*/
		virtual auto channels() -> uint32_t                                                                     = 0;
		/**
		* @brief Get the texture data
		*/
		virtual auto data() -> uint8_t*                                                                         = 0;
		/**
		* @brief Get the byte size of the texture
		*/
		virtual auto bytes() -> size_t                                                                          = 0;
		/**
		* @brief All the render apis provide ways to set gpu object names so that debuggers can display them (ie. render doc)
		* @param[in] str the object name
		* @note in release builds this will be a no-op. In debug builds this will be automatically called
		* 		setting the name to either the path or the resource id where appropriate.
		*/
		virtual auto set_debug_name(const std::string& str) -> void                                             = 0;
	};

	using TexturePtr = ResourcePtr<Texture, EResource::texture>;

} // namespace aby::rhi
