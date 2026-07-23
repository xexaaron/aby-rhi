#pragma once
#include "renderer.hpp"

namespace aby::rhi::vulkan {

    class Renderer : public aby::rhi::Renderer {
    public:
        auto init() -> bool override;
        auto deinit() -> void override;
        auto on_begin() -> bool override;
        auto on_end() -> void override;
        auto on_resize(uint32_t x, uint32_t y) -> void override;
    private:
    };

}