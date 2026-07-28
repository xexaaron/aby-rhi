#include "render-pass.hpp"
#include "backends/vulkan/vulkan-render-pass.hpp"
#include "context.hpp"

namespace aby::rhi {

    auto RenderPassBuilder::create() -> std::unique_ptr<RenderPassBuilder> {
        auto backend = Context::get().renderer_backend();
        switch (backend) {
            case ERenderer::vulkan: {
                return std::make_unique<vulkan::RenderPassBuilder>();
            }
            default:
                break;
        }
        return nullptr;
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