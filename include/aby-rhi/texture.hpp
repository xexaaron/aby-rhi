#pragma once
#include "common.hpp"
#include "resource.hpp"

#include <filesystem>

namespace aby::rhi {

	namespace fs = std::filesystem;


	class Texture {
	public:
		static constexpr uint32_t INVALID_ID = UINT32_MAX;
	public:
		static auto create(const fs::path& rel_path) -> ResourcePtr<Texture, EResource::texture>;
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
