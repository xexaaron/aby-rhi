#include "renderer.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"

namespace aby::rhi {

    auto IRenderer::create(ERenderer backend) -> IRenderer* {
        switch (backend) {
            case ERenderer::vulkan: 
                return new vulkan::Renderer();
            default:
                return nullptr;
        }
    }


}