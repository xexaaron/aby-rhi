#pragma once
#include "common.hpp"
#include "context.hpp"

namespace aby::rhi {

	class Texture {
	public:
		static constexpr uint32_t INVALID_ID = UINT32_MAX;
	public:
		static auto create(const fs::path& rel_path) -> std::shared_ptr<Texture>;
		virtual ~Texture() = default;

		virtual auto destroy() -> void      = 0;
		virtual auto id() -> uint32_t       = 0;
		virtual auto width() -> uint32_t    = 0;
		virtual auto height() -> uint32_t   = 0;
		virtual auto channels() -> uint32_t = 0;
		virtual auto data() -> uint8_t*     = 0;
		virtual auto bytes() -> size_t      = 0;
	};

} // namespace aby::rhi
