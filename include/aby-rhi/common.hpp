#pragma once
#include <algorithm>

#define aby_rhi_dbg(msg, ...) Context::get().logger()->log(ELogLevel::debug, std::format(msg __VA_OPT__(,) __VA_ARGS__))
#define aby_rhi_trc(msg, ...) Context::get().logger()->log(ELogLevel::trace, std::format(msg __VA_OPT__(,) __VA_ARGS__))
#define aby_rhi_log(msg, ...) Context::get().logger()->log(ELogLevel::info, std::format(msg __VA_OPT__(,) __VA_ARGS__))
#define aby_rhi_wrn(msg, ...) Context::get().logger()->log(ELogLevel::warn, std::format(msg __VA_OPT__(,) __VA_ARGS__))
#define aby_rhi_err(msg, ...) Context::get().logger()->log(ELogLevel::error, std::format(msg __VA_OPT__(,) __VA_ARGS__))
#define aby_rhi_ftl(msg, ...) Context::get().logger()->log(ELogLevel::fatal, std::format(msg __VA_OPT__(,) __VA_ARGS__))

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

    enum class EShader {
        none = 0,
        vert = 1,
        frag = 2,
        px   = frag,
        comp = 3,
        geom = 4,
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

    struct vec_tag {};

    template <typename T>
    struct vec2 : vec_tag {
        using underlying_type = T;

        template <typename U>
        requires (
            requires(const U& v) {
                v.x;
                v.y;
            } &&
            !std::derived_from<std::remove_cvref_t<U>, vec_tag>
        )
        vec2(const U& vec_type) : 
            x(static_cast<T>(vec_type.x)),
            y(static_cast<T>(vec_type.y)) {}
        vec2(T x, T y) : x(x), y(y) {}
        vec2(T s) : x(s), y(s) {}

        T x, y;
    };

    template <typename T>
    struct vec3 : vec_tag {
        using underlying_type = T;

        template <typename U>
        requires (
            requires(const U& v) {
                v.x;
                v.y;
                v.z;
            } &&
            !std::derived_from<std::remove_cvref_t<U>, vec_tag>
        )
        vec3(const U& vec_type) :
            x(static_cast<T>(vec_type.x)),
            y(static_cast<T>(vec_type.y)),
            z(static_cast<T>(vec_type.z)) {}
        vec3(T x, T y, T z) : x(x), y(y), z(z) {}
        vec3(T s) : x(s), y(s), z(s) {}

        T x, y, z;
    };

    template <typename T>
    struct vec4 : vec_tag {
        using underlying_type = T;

        template <typename U>
        requires (
            requires(const U& v) {
                v.x;
                v.y;
                v.z;
                v.w;
            } &&
            !std::derived_from<std::remove_cvref_t<U>, vec_tag>
        )
        vec4(const U& vec_type) :
            x(static_cast<T>(vec_type.x)),
            y(static_cast<T>(vec_type.y)),
            z(static_cast<T>(vec_type.z)),
            w(static_cast<T>(vec_type.w)) {}
        vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}
        vec4(T s) : x(s), y(s), z(s), w(s) {}

        T x, y, z, w;
    };

    struct mat4 {
        /// @brief Your mat4 type must be 16 contiguous floats only ie. glm::mat4 
        template <typename T>
        mat4(const T& mat_type) {
            std::memcpy(data, reinterpret_cast<float*>(*mat_type), sizeof(float) * 16);
        }

        float data[16];
    };

}