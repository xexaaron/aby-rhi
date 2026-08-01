#pragma once
#include "common.hpp"
#include "render-pass.hpp"

#include <cstdint>

namespace aby::rhi {

	class IRenderer {
	public:
		/**
         * @brief Create a renderer for the backend
         * @param backend a renderer api backend
         * @note  You are responsible for freeing this memory.
         */
		static auto create(ERenderer backend) -> IRenderer*;
		virtual ~IRenderer() = default;

		virtual auto init(void* native_window) -> bool                         = 0;
		virtual auto deinit() -> void                                          = 0;
		virtual auto add_pass(std::shared_ptr<RenderPass> render_pass) -> void = 0;
		virtual auto set_clear_color(Color color) -> void                      = 0;
		virtual auto on_begin() -> bool                                        = 0;
		virtual auto on_end() -> bool                                          = 0;
		virtual auto on_resize(uint32_t x, uint32_t y) -> void                 = 0;
	private:
	};

} // namespace aby::rhi
