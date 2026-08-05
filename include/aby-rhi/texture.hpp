#pragma once
#include "common.hpp"
#include "resource.hpp"

#include <filesystem>

namespace aby::rhi {

	namespace fs = std::filesystem;

	struct TextureParams {
		uint32_t mip_levels        = 0;   // if 0 then generate the mips based on the textures size. if 1 then no mips.
		float anisotropy_filtering = 0.f; // if 0, then disabled, otherwise clamped between the value and the devices maximum supported.
		EFiltering filtering       = EFiltering::linear;
		ERepeatMode repeat_mode    = ERepeatMode::repeat;
	};

	class Texture {
	public:
		static constexpr uint32_t INVALID_ID = UINT32_MAX;
	public:
		static auto create(const fs::path& rel_path, const TextureParams& params = {}) -> ResourcePtr<Texture, EResource::texture>;
		virtual ~Texture() = default;

		virtual auto destroy() -> void      = 0;
		/**
		 * @brief An id to the texture within an array or descriptor set that 
		 * 		  can be sent to the GPU to access bindless textures
		 */
		virtual auto id() -> uint32_t       = 0;
		virtual auto width() -> uint32_t    = 0;
		virtual auto height() -> uint32_t   = 0;
		virtual auto channels() -> uint32_t = 0;
		virtual auto data() -> uint8_t*     = 0;
		virtual auto bytes() -> size_t      = 0;
	};

	using TexturePtr = ResourcePtr<Texture, EResource::texture>;

} // namespace aby::rhi
