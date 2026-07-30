#pragma once
#include "common.hpp"
#include "shader.hpp"
#include "draw-cmd.hpp"

namespace aby::rhi {

    /// @brief Use the RenderPassBuilder to construct this.
    class RenderPass {
    public:
        /// @brief This is the only function that should be called by the user.
        ///        This must be called after Renderer::on_begin and before Renderer::on_end  
        auto submit(const DrawCmd& cmd) -> void;

        /// @brief The functions below should not be called by the user. only by the renderer backend.
        ///        these functions must be called during Renderer::on_begin 

        virtual auto bind() -> void = 0;
        virtual auto run() -> void = 0;
        virtual auto destroy() -> void = 0;
        virtual auto set_viewport(vec2<float> size, vec2<float> loc = {0.f, 0.f}, vec2<float> min_max_depth = {0.f, 1.f}) -> void = 0;
        virtual auto set_scissor(vec2<float> offset, vec2<float> size) -> void = 0;
    protected:
        std::vector<DrawCmd> m_Commands;
    };

    class RenderPassBuilder;

    struct VertexInput {
        EFormat format;
        size_t  offset;
    }; 

    class VertexInputDescriptionBuilder {
    private:
        template<typename>
        struct member_pointer_traits;

        template<typename Class, typename Member>
        struct member_pointer_traits<Member Class::*> {
            using class_type = Class;
            using member_type = Member;
        };

        template<typename T, auto Member>
        static size_t offset_of() {
            return reinterpret_cast<size_t>(
                &(reinterpret_cast<T const volatile*>(0)->*Member)
            );
        }
    public:
        VertexInputDescriptionBuilder(RenderPassBuilder* rpb);

        auto add_input(size_t bytes, EFormat format, size_t offset) -> VertexInputDescriptionBuilder&;
        /**
         * @brief Add a vertex shader input
         * @tparam Member in the format: &T::member
         * @param format The format corresponding to the members layout. (ie. vec2f -> rg_f32)
         */
        template<auto Member> requires(std::is_member_object_pointer_v<decltype(Member)>)
        auto add_input(EFormat format) -> VertexInputDescriptionBuilder& {
            using traits = member_pointer_traits<decltype(Member)>;
            using T = typename traits::class_type;
            using M = typename traits::member_type;

            m_Stride += sizeof(M);
            m_Inputs.emplace_back(format, offset_of<T, Member>());

            return *this;
        }
      
        
        auto build() -> RenderPassBuilder*;
        
        auto inputs() -> std::vector<VertexInput>&;
        auto stride() -> size_t;
    private:
        RenderPassBuilder*       m_RPB;
        std::vector<VertexInput> m_Inputs;
        size_t                   m_Stride;
    };

    class RenderPassBuilder {
    public:
        static auto create() -> std::unique_ptr<RenderPassBuilder>;
        RenderPassBuilder();
        virtual ~RenderPassBuilder() = default;

        virtual auto build() -> std::shared_ptr<RenderPass> = 0;

        /// @brief The render pass will own the created shader
        virtual auto add_shader(const fs::path& rel_path) -> RenderPassBuilder& = 0;
        virtual auto add_shader(std::shared_ptr<Shader> shader) -> RenderPassBuilder& = 0;
        
        auto vertex_description_builder() -> VertexInputDescriptionBuilder&; 
        
        virtual auto set_topology(ETopology topology) -> RenderPassBuilder& = 0;
        virtual auto set_polygon_mode(EPolygonMode mode) -> RenderPassBuilder& = 0;
        virtual auto set_cull_mode(ECullMode mode, EFrontFace front_face) -> RenderPassBuilder& = 0;

        virtual auto disable_multisampling() -> RenderPassBuilder& = 0;
        virtual auto disable_blending() -> RenderPassBuilder& = 0;
        virtual auto disable_depthtest() -> RenderPassBuilder& = 0;

        /// @brief ETopologoy::triangle_list  
        auto use_default_topology() -> RenderPassBuilder&;
        /// @brief EPolygonMode::fill
        auto use_default_polygon_mode(/*  */) -> RenderPassBuilder&;
        /// @brief ECullMode::none, EFrontFace::clockwise 
        auto use_default_cull_mode() -> RenderPassBuilder&;
    protected:
        VertexInputDescriptionBuilder m_VIDB;
    };



}