#pragma once
#include "common.hpp"
#include "draw-cmd.hpp"
#include "shader.hpp"
#include "texture.hpp"

#include <set>

namespace aby::rhi {

	/// @brief Use the RenderPassBuilder to construct this.
	class RenderPass {
	public:
		/**
         * @brief Submit a draw cmd to be rendered. This must be called each frame.
         * @param cmd The draw command.
         */
		auto submit(const DrawCmd& cmd) -> void;

		template <typename T>
		auto set_uniform(std::string_view name, const T& obj) -> void {
			set_uniform(name, &obj, sizeof(T));
		}

		virtual auto set_uniform(std::string_view name, const void* data, size_t bytes) -> void = 0;

		template <typename T>
		auto push_constant(std::string_view name, const T& obj) -> void {
			push_constant(name, &obj, sizeof(T));
		}

		virtual auto push_constant(std::string_view name, const void* data, size_t bytes) -> void = 0;

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
		virtual auto set_scissor(vec2<float> offset, vec2<float> size) -> void                                                        = 0;
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

		/// @brief The render pass will own the created shader
		virtual auto add_shader(const fs::path& rel_path) -> RenderPassBuilder&                                   = 0;
		virtual auto add_shader(ShaderPtr shader) -> RenderPassBuilder&                                           = 0;
		virtual auto add_uniform(std::string_view name, uint32_t binding, EShader stage) -> RenderPassBuilder&    = 0;
		virtual auto add_color_attachment(Resource texture, bool is_present_target = false) -> RenderPassBuilder& = 0;
		/**
		 * @brief Add a push constant to the render pass
		 * @param name The name to access it by
		 * @param bytes The size of the push constant
		 * @note Push constants are globally accessible by all shader stages
		 */
		virtual auto add_push_constant(const std::string& name, size_t bytes) -> RenderPassBuilder&               = 0;
		/**
		 * @brief Add a push constant to the render pass
		 * @tparam T the type of the object to be used for the size of the push constant
		 * @param name The name to access it by
		 * @note Push constants are globally accessible by all shader stages
		 */
		template <typename T>
		auto add_push_constant(const std::string& name) -> RenderPassBuilder&;
		/**
         * @brief add a vertex input
         * @param bytes The size of the vertex member.
         * @param format The format of the member (ie. vec2f -> rg_f32)
         * @param offset The offsetof the member compared to the Vertex structure. 
        */
		virtual auto add_vertex_input(size_t bytes, EFormat format, size_t offset) -> RenderPassBuilder& = 0;
		/**
         * @brief Add a vertex shader input
         * @tparam Member in the format: &T::member
         * @param format The format corresponding to the members layout. (ie. vec2f -> rg_f32)
         */
		template <auto Member>
		requires(std::is_member_object_pointer_v<decltype(Member)>)
		auto add_vertex_input(EFormat format) -> RenderPassBuilder&;
		/**
         * @brief Add vertex shader inputs
         * @tparam ...Member in the format: &T::member...
         * @param formats The format(s) corresponding to the members layout. (ie. vec2f -> rg_f32)
         */
		template <auto... Member>
		requires((std::is_member_object_pointer_v<decltype(Member)> && ...))
		auto add_vertex_inputs(std::same_as<EFormat> auto... formats) -> RenderPassBuilder&;

		virtual auto set_topology(ETopology topology) -> RenderPassBuilder&                                      = 0;
		virtual auto set_polygon_mode(EPolygonMode mode, float line_width) -> RenderPassBuilder&                 = 0;
		virtual auto set_cull_mode(ECullMode mode, EFrontFace front_face) -> RenderPassBuilder&                  = 0;
		virtual auto set_depth_format(EFormat format) -> RenderPassBuilder&                                      = 0;
		virtual auto set_depth(bool enable_test, bool enable_write, ECompareOp compare_op) -> RenderPassBuilder& = 0;
		virtual auto set_stencil(bool enable, ECompareOp compare_op) -> RenderPassBuilder&                       = 0;
		virtual auto set_blend_color(bool enable, Blend blend, size_t attachment = 0) -> RenderPassBuilder&      = 0;
		auto set_blend_color(bool enable, Blend blend, std::set<size_t> attachments) -> RenderPassBuilder&;
		virtual auto set_blend_alpha(Blend blend, size_t attachment = 0) -> RenderPassBuilder& = 0;
		auto set_blend_alpha(Blend blend, std::set<size_t> attachments) -> RenderPassBuilder&;
		virtual auto set_blend_mask(EChannels mask, size_t attachment = 0) -> RenderPassBuilder& = 0;
		auto set_blend_mask(EChannels mask, std::set<size_t> attachments) -> RenderPassBuilder&;
		virtual auto set_antialiasing(EAntiAliasing aliasing) -> RenderPassBuilder& = 0;

		virtual auto disable_blending() -> RenderPassBuilder&  = 0;
		virtual auto disable_depthtest() -> RenderPassBuilder& = 0;

		// use_default_topology, use_default_polygon_mode, not present, ... etc.
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
