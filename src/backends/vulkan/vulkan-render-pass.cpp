#include "backends/vulkan/vulkan-render-pass.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"
#include "backends/vulkan/vulkan-shader.hpp"
#include "backends/vulkan/vulkan-common.hpp"
#include "backends/vulkan/vulkan-buffer.hpp"

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
        for (auto& cmd : m_Commands) {
            auto* i = static_cast<vulkan::IndexBuffer*>(cmd.ibuff());
            auto* v = static_cast<vulkan::VertexBuffer*>(cmd.vbuff());
            auto  s = cmd.instances();

            VkBuffer vb = v->gpu();
            VkBuffer ib = i->gpu();
            VkDeviceSize vb_offset = 0;

            vkCmdBindVertexBuffers(m_Cmd, 0, 1, &vb, &vb_offset);
            vkCmdBindIndexBuffer(m_Cmd, ib, 0, VK_INDEX_TYPE_UINT32);
            vkCmdDrawIndexed(m_Cmd, i->count(), s, 0, 0, 0);
        }
    }

    auto RenderPass::destroy() -> void {
        for (auto& shader : m_Shaders) {
            shader->destroy();
        }
        for (auto& cmd : m_Commands) {
            cmd.vbuff()->destroy();
            cmd.ibuff()->destroy();
        }
        m_Pipeline->destroy();
        
    }

    auto RenderPass::set_viewport(vec2<float> size, vec2<float> loc, vec2<float> min_max_depth) -> void {
        /* flip the y for vulkan rendering */
        vk::Viewport vp(loc.x, size.y, size.x, -size.y, min_max_depth.x, min_max_depth.y);
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

        if (!m_VIDB.inputs().empty()) {
            m_PipelineBuilder.add_vertex_type(0, m_VIDB.stride());
            auto& inputs = m_VIDB.inputs();

            for (size_t i = 0; i < inputs.size(); i++) {
                auto& input = inputs[i];
                vk::Format format = eformat_to_vkformat(input.format);
                m_PipelineBuilder.add_vertex_field(i, 0, format, inputs[i].offset);
            }
        }

        auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

        m_PipelineBuilder.set_color_attachment_format(r->color_format());
        m_PipelineBuilder.set_depth_format(vk::Format::eUndefined);

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