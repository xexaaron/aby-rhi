#pragma once
#include <cstdint>

namespace aby::rhi {

    class IRenderer {
    public:
        virtual auto init(void* native_window) -> bool = 0;
        virtual auto deinit() -> void = 0;
        virtual auto on_begin() -> bool = 0;
        virtual auto on_end() -> void = 0;
        virtual auto on_resize(uint32_t x, uint32_t y) -> void = 0;
    private:
    };

}