#pragma once
#include "common.hpp"
#include "render-pass.hpp"

#include <cstdint>

namespace aby::rhi {

	struct GraphicsParams {
	};

	class ABY_RHI_API Renderer {
	public:
		Renderer()                = default;
		Renderer(const Renderer&) = delete;
		Renderer(Renderer&&)      = delete;
		/**
         * @brief Create a renderer for the backend
         * @param[in] backend a renderer api backend
		 * @param[in] graphics graphics parameters
         */
		static auto create(ERenderer backend, const GraphicsParams& graphics) -> Renderer*;
		virtual ~Renderer()                                                    = default;
		/**
		* @brief Initialize the renderer backend
		* @param[in] native_window The operating system native window handles
		* @return true on success, otherwise false
		*/
		virtual auto init(void* native_window) -> bool                         = 0;
		/**
		* @brief Deinitialize the renderer backend
		*/
		virtual auto deinit() -> void                                          = 0;
		/**
		* @brief Add a render pass created with @c RenderPassBuilder
		* @param[in] render_pass the render pass to add
		*/
		virtual auto add_pass(std::shared_ptr<RenderPass> render_pass) -> void = 0;
		/**
		* @brief Set the renderer clear color
		* @param[in] color an rgba float[4] color
		*/
		virtual auto set_clear_color(Color color) -> void                      = 0;
		/**
		* @brief Begin the renderer frame loop
		* @note all rendering operations must be within this and @c Renderer::on_end
		* @return false if the sync primitives failed or commands tracking failed to begin, otherwise true 
		*/
		virtual auto on_begin() -> bool                                        = 0;
		/**
		* @brief End the renderer frame loop
		* @note no rendering operations can happen again until @c Renderer::on_begin is called
		* @return false if the swapchain was out of date and had to be recreated, otherwise true
		*/
		virtual auto on_end() -> bool                                          = 0;
	private:
	};

} // namespace aby::rhi
