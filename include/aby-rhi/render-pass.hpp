#pragma once
#include "common.hpp"
#include "draw-cmd.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "utility/meta.hpp"

#include <set>

namespace aby::rhi {

	/// @brief Use the RenderPassBuilder to construct this.
	class ABY_RHI_API RenderPass {
	public:
		RenderPass()                  = default;
		RenderPass(const RenderPass&) = delete;
		RenderPass(RenderPass&&)      = delete;

		/**
         * @brief Submit a draw cmd to be rendered. This must be called each frame.
         * @param[in] cmd The draw command.
         */
		auto submit(const DrawCmd& cmd) -> void;
		/**
		* @brief Set a uniform by name
		* @param[in] name The name of the uniform (set during RenderPassBuilder creation)
		* @param[in] obj the data to set the uniform to
		*/
		template <typename T>
		auto set_uniform(std::string_view name, const T& obj) -> void {
			set_uniform(name, &obj, sizeof(T));
		}
		/**
		* @brief Set a uniform by name
		* @param[in] name The name of the uniform (set during RenderPassBuilder creation) 
		* @param[in] data the data to set the uniform to
		* @param[in] bytes the size of the data and the uniform (they must match)
		*/
		virtual auto set_uniform(std::string_view name, const void* data, size_t bytes) -> void = 0;
		/**
		* @brief Upload a constant by name
		* @param[in] name The name of the constant (set during RenderPassBuilder creation)
		* @param[in] obj The data to set the constant to
		*/
		template <typename T>
		auto push_constant(std::string_view name, const T& obj) -> void {
			push_constant(name, &obj, sizeof(T));
		}
		/**
		* @brief Upload a constant by name
		* @param[in] name The name of the constant (set during RenderPassBuilder creation)
		* @param[in] data The data to set the constant to
		* @param[in] bytes The size of the data and the constant (they must match)
		*/
		virtual auto push_constant(std::string_view name, const void* data, size_t bytes) -> void = 0;
		/**
		* @brief Set the scissor flag
		* @param[in] enabled [true | false] 
		*/
		virtual auto set_scissor_enable(bool enabled) -> void                                     = 0;
		/**
		* @brief Set the scissor region to use if scissor is enabled
		* @param[in] min the min coords of the rectangle
		* @param[in] max the max coords of the rectangle
		*/
		virtual auto set_scissor_region(vec2<int> min, vec2<int> max) -> void                     = 0;

		/// @brief The functions below should not be called by the user. only by the renderer backend.
		///        these functions must be called during Renderer::on_begin

		/**
         * @brief Clear the list of commands
         */
		auto clear() -> void;
		/**
         * @brief bind the pipeline, buffers, and any other resources used.
         */
		virtual auto bind() -> void                                                                                                   = 0;
		/**
		 * @brief begin rendering using attachments
		*/
		virtual auto begin() -> void                                                                                                  = 0;
		/** 
		 * @brief end rendering using attachments
		*/
		virtual auto end() -> void                                                                                                    = 0;
		/**
         * @brief executes each draw command.
         */
		virtual auto run() -> void                                                                                                    = 0;
		/**
         * @brief destroy all resources the RenderPass owns. 
        */
		virtual auto destroy() -> void                                                                                                = 0;
		/**
         * @brief set the viewport. In backends like vulkan it will be configured to use loc=0,0 as the top left.
         */
		virtual auto set_viewport(vec2<float> size, vec2<float> loc = { 0.f, 0.f }, vec2<float> min_max_depth = { 0.f, 1.f }) -> void = 0;
		/**
         * @brief set the scissor. 
         */
		virtual auto set_scissor() -> void                                                                                            = 0;
	protected:
		std::vector<DrawCmd> m_Commands;
	};

	class RenderPassBuilder {
	public:
		static auto create() -> std::unique_ptr<RenderPassBuilder>;
		RenderPassBuilder();
		virtual ~RenderPassBuilder() = default;

		virtual auto build() -> std::shared_ptr<RenderPass> = 0;
		virtual auto clear() -> void                        = 0;

		/**
		* @brief Add a shader to the render pass
		* @param[in] rel_path a path relative to the CWD
		* @note The shader will be compiled in the background, and it will block if not finished compiling
		* 		when @c RenderPassBuilder::build() is called
		*/
		virtual auto add_shader(const fs::path& rel_path) -> RenderPassBuilder&                                          = 0;
		/**
		* @brief Add a shader to the render pass
		* @param[in] name the name of the shader (used for caching)
		* @param[in] source_code the shader glsl source code
		* @param[in] type the shader stage
		* @note The shader will be compiled in the background, and it will block if not finished compiling
		* 		when @c RenderPassBuilder::build() is called
		*/
		virtual auto add_shader(std::string_view name, std::string_view source_code, EShader type) -> RenderPassBuilder& = 0;
		/**
		* @brief Add a shader to the render pass
		* @param[in] shader a resource shader ptr
		* @note The shader if still compiling in the background will block if not finished compiling
		* 		when @c RenderPassBuilder::build() is called 
		*/
		virtual auto add_shader(ShaderPtr shader) -> RenderPassBuilder&                                                  = 0;
		/**
		* @brief Add a shader uniform to the render pass
		* @param[in] name the name of the uniform, this should generally correspond to the name in the shader but does not have to.
		* @param[in] binding the binding of the uniform in the shader
		* @param[in] stage the shader stage that contains the uniform
		*/
		virtual auto add_uniform(std::string_view name, uint32_t binding, EShader stage) -> RenderPassBuilder&           = 0;
		/**
		* @brief Add a color attachment to the render pass
		* @param[in] texture a texture created via @c Texture::create_render_target(...) If the texture has not finished
						 loading by the time @c RenderPassBuilder::build() is called it will block until finished loading.
		* @param[in] is_present_target the present target is the final image to be presented to the swapchain between ALL passes. 
		*/
		virtual auto add_color_attachment(Resource texture, bool is_present_target = false) -> RenderPassBuilder&        = 0;
		/**
		 * @brief Add a push constant to the render pass
		 * @param[in] name The name to access it by
		 * @param[in] bytes The size of the push constant
		 * @note Push constants are globally accessible by all shader stages
		 */
		virtual auto add_push_constant(const std::string& name, size_t bytes) -> RenderPassBuilder&                      = 0;
		/**
		 * @brief Add a push constant to the render pass
		 * @tparam T the type of the object to be used for the size of the push constant
		 * @param[in] name The name to access it by
		 * @note Push constants are globally accessible by all shader stages
		 */
		template <typename T>
		auto add_push_constant(const std::string& name) -> RenderPassBuilder&;
		/**
         * @brief add a vertex input
         * @param[in] bytes The size of the vertex member.
         * @param[in] format The format of the member (ie. vec2f -> rg_f32)
         * @param[in] offset The offsetof the member compared to the Vertex structure. 
        */
		virtual auto add_vertex_input(size_t bytes, EFormat format, size_t offset) -> RenderPassBuilder& = 0;
		/**
         * @brief Add a vertex shader input
         * @tparam Member in the format: &T::member
         * @param[in] format The format corresponding to the members layout. (ie. vec2f -> rg_f32)
         */
		template <auto Member>
		requires(std::is_member_object_pointer_v<decltype(Member)>)
		auto add_vertex_input(EFormat format) -> RenderPassBuilder&;
		/**
         * @brief Add vertex shader inputs
         * @tparam ...Member in the format: &T::member...
         * @param[in] formats The format(s) corresponding to the members layout. (ie. vec2f -> rg_f32)
         */
		template <auto... Member>
		requires((std::is_member_object_pointer_v<decltype(Member)> && ...))
		auto add_vertex_inputs(std::same_as<EFormat> auto... formats) -> RenderPassBuilder&;

		/**
		* @brief Set the topology mode
		* @param[in] topology the mode
		*/
		virtual auto set_topology(ETopology topology) -> RenderPassBuilder&                                      = 0;
		/**
		* @brief Set the polygon mode
		* @param[in] mode the mode
		* @param[in] line_width the width of the polygon lines
		*/
		virtual auto set_polygon_mode(EPolygonMode mode, float line_width) -> RenderPassBuilder&                 = 0;
		/**
		* @brief Set the cull mode
		* @param[in] mode the mode
		* @param[in] front_face the triangle winding order
		*/
		virtual auto set_cull_mode(ECullMode mode, EFrontFace front_face) -> RenderPassBuilder&                  = 0;
		/**
		* @brief Set the depth format of the depth attachment
		* @param[in] format the format 
		*/
		virtual auto set_depth_format(EFormat format) -> RenderPassBuilder&                                      = 0;
		/**
		* @brief Set the depth enabling flags and comparison operation
		* @param[in] enable_test enable depth testing in the attachments
		* @param[in] enable_write enable depth writing in the attachments
		* @param[in] compare_op the comparison op for depth operations
		*/
		virtual auto set_depth(bool enable_test, bool enable_write, ECompareOp compare_op) -> RenderPassBuilder& = 0;
		/**
		* @brief Set stencil enabling flags and comparison operation
		* @param[in] enable enable stencil in the attachments
		* @param[in] compare_op the comparison op for stencil operations
		*/
		virtual auto set_stencil(bool enable, ECompareOp compare_op) -> RenderPassBuilder&                       = 0;
		/**
		* @brief Set the blend color enabling flags, operations, and factors for a color attachment
		* @param[in] enable enable color blending in the color attachment
		* @param[in] blend the color blend operation and blend factors
		* @param[in] attachment the attachment to set these values for
		*/
		virtual auto set_blend_color(bool enable, Blend blend, size_t attachment = 0) -> RenderPassBuilder&      = 0;
		/**
		* @brief Set the blend color enabling flags, operations, and factors for color attachment(s)
		* @param[in] enable enable color blending in the color attachment
		* @param[in] blend the color blend operation and blend factors
		* @param[in] attachments the attachment(s) to set these values for
		*/
		auto set_blend_color(bool enable, Blend blend, std::set<size_t> attachments) -> RenderPassBuilder&;
		/**
		* @brief Set the blend alpha operations, and factors for a color attachment
		* @param[in] blend the alpha blend operation and blend factors
		* @param[in] attachment the attachment to set these values for
		*/
		virtual auto set_blend_alpha(Blend blend, size_t attachment = 0) -> RenderPassBuilder& = 0;
		/**
		* @brief Set the blend alpha operations, and factors for color attachment(s)
		* @param[in] blend the alpha blend operation and blend factors
		* @param[in] attachments the attachment(s) to set these values for
		*/
		auto set_blend_alpha(Blend blend, std::set<size_t> attachments) -> RenderPassBuilder&;
		/**
		* @brief Set the blend mask for a color attachment
		* @param[in] mask [R|G|B|A]
		* @param[in] attachment the attachment to set the mask for
		*/
		virtual auto set_blend_mask(EChannels mask, size_t attachment = 0) -> RenderPassBuilder& = 0;
		/**
		* @brief Set the blend mask for color attachment(s)
		* @param[in] mask [R|G|B|A]
		* @param[in] attachments the attachment(s) to set the mask for
		*/
		auto set_blend_mask(EChannels mask, std::set<size_t> attachments) -> RenderPassBuilder&;
		/**
		* @brief Set the anti-aliasing factor for the attachments
		* @note the anti-aliasing must be the same as each color attachment in the render pass
		*/
		virtual auto set_antialiasing(EAntiAliasing aliasing) -> RenderPassBuilder& = 0;
		/// @brief disable all blending operations
		virtual auto disable_blending() -> RenderPassBuilder&                       = 0;
		/// @brief disable all depth operations
		virtual auto disable_depthtest() -> RenderPassBuilder&                      = 0;
		/// @brief use_default_topology, use_default_polygon_mode, not present, ... etc.
		auto use_all_defaults() -> RenderPassBuilder&;
		/// @brief ETopologoy::triangle_list
		auto use_default_topology() -> RenderPassBuilder&;
		/// @brief EPolygonMode::fill
		auto use_default_polygon_mode(/*  */) -> RenderPassBuilder&;
		/// @brief ECullMode::none, EFrontFace::clockwise
		auto use_default_cull_mode() -> RenderPassBuilder&;
		/// @brief Renderer determinant
		virtual auto use_default_attachment_formats() -> RenderPassBuilder& = 0;
	};

} // namespace aby::rhi

namespace aby::rhi {

	template <typename T>
	auto RenderPassBuilder::add_push_constant(const std::string& name) -> RenderPassBuilder& {
		return add_push_constant(name, sizeof(T));
	}

	template <auto Member>
	requires(std::is_member_object_pointer_v<decltype(Member)>)
	auto RenderPassBuilder::add_vertex_input(EFormat format) -> RenderPassBuilder& {
		using traits = meta::member_pointer_traits<decltype(Member)>;
		using T      = typename traits::class_type;
		using M      = typename traits::member_type;
		add_vertex_input(sizeof(M), format, meta::offset_of<T, Member>());
		return *this;
	}

	template <auto... Member>
	requires((std::is_member_object_pointer_v<decltype(Member)> && ...))
	auto RenderPassBuilder::add_vertex_inputs(std::same_as<EFormat> auto... formats) -> RenderPassBuilder& {
		static_assert(sizeof...(Member) == sizeof...(formats));
		(add_vertex_input<Member>(formats), ...);
		return *this;
	}

} // namespace aby::rhi
