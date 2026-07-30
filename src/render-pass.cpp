#include "render-pass.hpp"
#include "backends/vulkan/vulkan-render-pass.hpp"
#include "context.hpp"

namespace aby::rhi {

    auto RenderPass::submit(const DrawCmd& cmd) -> void {
        m_Commands.push_back(cmd);
    }

    auto RenderPass::clear() -> void {
        m_Commands.clear();
    }

}

namespace aby::rhi {
    
    VertexInputDescriptionBuilder::VertexInputDescriptionBuilder(RenderPassBuilder* rpb) :
        m_RPB(rpb)
    {

    }

    auto VertexInputDescriptionBuilder::add_input(size_t bytes, EFormat format, size_t offset) -> VertexInputDescriptionBuilder& {
        m_Stride += bytes;
        m_Inputs.emplace_back(format, offset);
        return *this;
    }
    
    auto VertexInputDescriptionBuilder::build() -> RenderPassBuilder* {
        return m_RPB;
    }
    
    auto VertexInputDescriptionBuilder::inputs() -> std::vector<VertexInput>& {
        return m_Inputs;
    }

    auto VertexInputDescriptionBuilder::stride() -> size_t {
        return m_Stride;
    }


}

namespace aby::rhi {

    auto RenderPassBuilder::create() -> std::unique_ptr<RenderPassBuilder> {
        auto backend = Context::get().renderer_backend();
        switch (backend) {
            case ERenderer::vulkan: {
                return std::make_unique<vulkan::RenderPassBuilder>();
            }
            default:
                aby_rhi_assert(false, "unimplemented renderer backend: {}", backend);
        }
        return nullptr;
    }

    RenderPassBuilder::RenderPassBuilder() : m_VIDB(this) {}

    auto RenderPassBuilder::vertex_description_builder() -> VertexInputDescriptionBuilder& {
        return m_VIDB;
    } 

    auto RenderPassBuilder::use_default_topology() -> RenderPassBuilder& {
        return set_topology(ETopology::triangle_list);
    }
    
    auto RenderPassBuilder::use_default_polygon_mode() -> RenderPassBuilder& {
        return set_polygon_mode(EPolygonMode::fill);
    }
    
    auto RenderPassBuilder::use_default_cull_mode() -> RenderPassBuilder& {
        return set_cull_mode(ECullMode::none, EFrontFace::clockwise);
    }

}