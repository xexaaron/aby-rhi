#pragma once
#include "common.hpp"
#include "shader.hpp"

namespace aby::rhi {

    enum class ETopology {
        point_list,
        line_list,
        line_strip,
        triangle_list,
        triangle_strip,
        triangle_fan,
    };

    enum class EPolygonMode {
        fill,
        line,
        point,
    };

    enum class ECullMode {
        none,
        front,
        back,
        front_and_back,
    };

    enum class EFrontFace {
        clockwise,
        counter_clockwise,
    };

    enum class EFormat {
        none,
        rgba_sf16,
    };

    /// @brief Use the RenderPassBuilder to construct this.
    class RenderPass {
    public:
        /// @brief Called by the renderer backend
        virtual auto bind() -> void = 0;
        /// @brief Called by the renderer backend
        virtual auto destroy() -> void = 0;
        /// @brief Called by the renderer backend
        virtual auto set_viewport(vec2<float> size, vec2<float> loc = {0.f, 0.f}, vec2<float> min_max_depth = {0.f, 1.f}) -> void = 0;
        /// @brief Called by the renderer backend
        virtual auto set_scissor(vec2<float> offset, vec2<float> size) -> void = 0;
    private:
    };

    class RenderPassBuilder {
    public:
        static auto create() -> std::unique_ptr<RenderPassBuilder>;
        virtual ~RenderPassBuilder() = default;

        virtual auto build() -> std::shared_ptr<RenderPass> = 0;

        virtual auto add_shader(std::shared_ptr<Shader> shader) -> RenderPassBuilder& = 0;
        
        virtual auto set_topology(ETopology topology) -> RenderPassBuilder& = 0;
        virtual auto set_polygon_mode(EPolygonMode mode) -> RenderPassBuilder& = 0;
        virtual auto set_cull_mode(ECullMode mode, EFrontFace front_face) -> RenderPassBuilder& = 0;
        virtual auto set_color_attachment_format(EFormat format) -> RenderPassBuilder& = 0;
        virtual auto set_depth_format(EFormat format) -> RenderPassBuilder& = 0; 

        virtual auto disable_multisampling() -> RenderPassBuilder& = 0;
        virtual auto disable_blending() -> RenderPassBuilder& = 0;
        virtual auto disable_depthtest() -> RenderPassBuilder& = 0;

        auto use_default_topology(/* ETopology::triangle_list */) -> RenderPassBuilder&;
        auto use_default_polygon_mode(/* EPolygonMode::fill */) -> RenderPassBuilder&;
        auto use_default_cull_mode(/* ECullMode::none, EFrontFace::clockwise */) -> RenderPassBuilder&;
    };



}