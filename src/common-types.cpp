#include "common-types.hpp"

namespace aby::rhi {

    Color::Color(float scalar) {
        scalar = std::clamp(scalar, 0.f, 1.f);
        for (size_t i = 0; i < 3; i++) {
            this->rgba[i] = scalar;
        }
        this->a = 1.f;
    }
            
    Color::Color(float r, float g, float b, float a) {
        this->r = std::clamp(r, 0.f, 1.f);
        this->g = std::clamp(g, 0.f, 1.f);
        this->b = std::clamp(b, 0.f, 1.f);
        this->a = std::clamp(a, 0.f, 1.f);
    }

}