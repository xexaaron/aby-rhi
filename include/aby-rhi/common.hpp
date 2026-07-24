#pragma once
#include <algorithm>

namespace aby::rhi {

    enum class EAllocation {
        backend,
        interface,
    };

    enum class ERenderer {
        vulkan,
    };

    enum class EWindow {
        win32,
        x11,
        xcb,
        wayland,
        metal
    };

    struct Color {
        Color(float scalar = 0.f) {
            scalar = std::clamp(scalar, 0.f, 1.f);
            for (size_t i = 0; i < 3; i++) {
                this->rgba[i] = scalar;
            }
            this->a = 1.f;
        }
            
        Color(float r, float g, float b, float a = 1.f) {
            this->r = std::clamp(r, 0.f, 1.f);
            this->g = std::clamp(g, 0.f, 1.f);
            this->b = std::clamp(b, 0.f, 1.f);
            this->a = std::clamp(a, 0.f, 1.f);
        }

        union {
            float rgba[4];
            struct {
                float r;
                float g;
                float b;
                float a;
            };
        };
      
    };

}