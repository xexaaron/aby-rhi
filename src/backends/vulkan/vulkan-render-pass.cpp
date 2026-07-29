#include "backends/vulkan/vulkan-render-pass.hpp"
#include "backends/vulkan/vulkan-shader.hpp"

namespace aby::rhi::vulkan {

    RenderPass::RenderPass(std::unique_ptr<Pipeline> pipeline,  const std::vector<std::shared_ptr<rhi::Shader>>& shaders) :
        m_BindPoint(vk::PipelineBindPoint::eGraphics),
        m_Cmd(VK_NULL_HANDLE),
        m_Pipeline(std::move(pipeline)),
        m_Shaders(shaders)
    {
        
    }

    auto RenderPass::bind() -> void {
        m_Pipeline->bind(m_Cmd, m_BindPoint);
    }

    auto RenderPass::run() -> void {
        
    }

    auto RenderPass::destroy() -> void {
        for (auto& shader : m_Shaders) {
            shader.reset();
        }
        m_Pipeline->destroy();
    }

    auto RenderPass::set_viewport(vec2<float> size, vec2<float> loc, vec2<float> min_max_depth) -> void {
        vk::Viewport vp(loc.x, loc.y, size.x, size.y, min_max_depth.x, min_max_depth.y);
        vkCmdSetViewport(
            m_Cmd,
            0, /* first viewport */
            1, /* viewport count */
            reinterpret_cast<VkViewport*>(&vp)
        );
    }
    
    auto RenderPass::set_scissor(vec2<float> offset, vec2<float> size) -> void {
        vk::Rect2D scissor(
            vk::Offset2D(offset.x, offset.y),
            vk::Extent2D(size.x, size.y)
        );
        vkCmdSetScissor(
            m_Cmd,
            0,
            1,
            reinterpret_cast<VkRect2D*>(&scissor)
        );
    }

    auto RenderPass::set_bind_point(vk::PipelineBindPoint point) -> RenderPass& {
        m_BindPoint = point;
        return *this;
    }

    auto RenderPass::set_cmd_buffer(vk::CommandBuffer cmd) -> RenderPass& {
        m_Cmd = cmd;
        return *this;
    }

}

namespace aby::rhi::vulkan {

    auto RenderPassBuilder::build() -> std::shared_ptr<rhi::RenderPass> {
        return std::make_shared<RenderPass>(
            std::move(m_PipelineBuilder.build()),
            m_Shaders
        );
    }

    auto RenderPassBuilder::add_shader(const fs::path& rel_path) -> RenderPassBuilder& {
        auto shader = Shader::create(rel_path);
        return add_shader(shader);
    }

    auto RenderPassBuilder::add_shader(std::shared_ptr<rhi::Shader> shader) -> RenderPassBuilder& {
        auto s = std::static_pointer_cast<vulkan::Shader>(shader);
        m_PipelineBuilder.add_shader(s->type(), s->module());
        m_Shaders.push_back(shader);
        return *this;
    }

    auto RenderPassBuilder::set_topology(ETopology topology) -> RenderPassBuilder& {
        vk::PrimitiveTopology t;
        switch (topology) {
            case ETopology::point_list:     t = vk::PrimitiveTopology::ePointList;     break;
            case ETopology::line_list:      t = vk::PrimitiveTopology::eLineList;      break;
            case ETopology::line_strip:     t = vk::PrimitiveTopology::eLineStrip;     break;
            case ETopology::triangle_list:  t = vk::PrimitiveTopology::eTriangleList;  break;
            case ETopology::triangle_strip: t = vk::PrimitiveTopology::eTriangleStrip; break;
            case ETopology::triangle_fan:   t = vk::PrimitiveTopology::eTriangleFan;   break;
        }
        m_PipelineBuilder.set_topology(t);
        return *this;
    }

    auto RenderPassBuilder::set_polygon_mode(EPolygonMode mode) -> RenderPassBuilder& {
        vk::PolygonMode m;
        switch (mode) {
            case EPolygonMode::fill:  m = vk::PolygonMode::eFill;  break; 
            case EPolygonMode::line:  m = vk::PolygonMode::eLine;  break;
            case EPolygonMode::point: m = vk::PolygonMode::ePoint; break;
        }
        m_PipelineBuilder.set_polygon_mode(m);
        return *this;
    }
    
    auto RenderPassBuilder::set_cull_mode(ECullMode mode, EFrontFace front_face) -> RenderPassBuilder& {
        vk::CullModeFlags m;
        vk::FrontFace     f;

        switch (mode) {
            case ECullMode::none:           m = vk::CullModeFlagBits::eNone;         break;
            case ECullMode::front:          m = vk::CullModeFlagBits::eFront;        break;
            case ECullMode::back:           m = vk::CullModeFlagBits::eBack;         break;
            case ECullMode::front_and_back: m = vk::CullModeFlagBits::eFrontAndBack; break;
        }

        switch (front_face) {
            case EFrontFace::clockwise:         f = vk::FrontFace::eClockwise;        break;
            case EFrontFace::counter_clockwise: f = vk::FrontFace::eCounterClockwise; break;
        }

        m_PipelineBuilder.set_cull_mode(m, f);
        return *this;
    }
    
    auto RenderPassBuilder::set_color_attachment_format(EFormat format) -> RenderPassBuilder& {
        vk::Format f;
        switch (format) {
            case EFormat::none:      f = vk::Format::eUndefined;          break;
            case EFormat::rgba_sf16: f = vk::Format::eR16G16B16A16Sfloat; break;
        }                                            
        m_PipelineBuilder.set_color_attachment_format(f);
        return *this;
    }
    
    auto RenderPassBuilder::set_depth_format(EFormat format) -> RenderPassBuilder& {
        vk::Format f;
        switch (format) {
            case EFormat::none:      f = vk::Format::eUndefined;          break;
            case EFormat::rgba_sf16: f = vk::Format::eR16G16B16A16Sfloat; break;
        }
        m_PipelineBuilder.set_depth_format(f);
        return *this;
    }

    auto RenderPassBuilder::disable_multisampling() -> RenderPassBuilder& {
        m_PipelineBuilder.disable_multisampling();
        return *this;
    }

    auto RenderPassBuilder::disable_blending() -> RenderPassBuilder& {
        m_PipelineBuilder.disable_blending();
        return *this;
    }

    auto RenderPassBuilder::disable_depthtest() -> RenderPassBuilder& {
        m_PipelineBuilder.disable_depthtest();
        return *this;
    }

}