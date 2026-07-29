#pragma once
#include <algorithm>

#if defined(_MSC_VER)
    #define ABY_RHI_PACK_PUSH __pragma(pack(push, 1))
    #define ABY_RHI_PACK_POP  __pragma(pack(pop))
    #define ABY_RHI_PACKED_TYPE
#elif defined(__GNUC__) || defined(__clang__)
    #define ABY_RHI_PACK_PUSH
    #define ABY_RHI_PACK_POP
    #define ABY_RHI_PACKED_TYPE __attribute__((packed))
#else
    #define ABY_RHI_PACK_PUSH
    #define ABY_RHI_PACK_POP
    #define ABY_RHI_PACKED_TYPE
#endif

#define ABY_RHI_PACKED(structure) ABY_RHI_PACK_PUSH structure ABY_RHI_PACKED_TYPE; ABY_RHI_PACK_POP 

namespace aby::rhi {

    ABY_RHI_PACKED(struct Color {
        Color(float scalar = 0.f);
        Color(float r, float g, float b, float a = 1.f);

        union {
            float rgba[4];
            struct {
                float r;
                float g;
                float b;
                float a;
            };
        };
    });

    ABY_RHI_PACKED(struct vec_tag {});

    ABY_RHI_PACKED(template <typename T>
        struct vec2 : vec_tag {
        using underlying_type = T;

        template <typename U> requires (requires(const U& v) { v.x; v.y; } && !std::derived_from<std::remove_cvref_t<U>, vec_tag>)
        vec2(const U& vec_type);
        vec2(T x, T y);
        vec2(T s);

        T x;
        T y;
    });

    ABY_RHI_PACKED(template <typename T>
    struct vec3 : vec_tag {
        using underlying_type = T;

        template <typename U> requires (requires(const U& v) { v.x; v.y; v.z; } && !std::derived_from<std::remove_cvref_t<U>, vec_tag>)
        vec3(const U& vec_type);
        vec3(T x, T y, T z);
        vec3(T s);

        T x;
        T y;
        T z;
    });

    ABY_RHI_PACKED(template <typename T>
    struct vec4 : vec_tag {
        using underlying_type = T;

        template <typename U> requires (requires(const U& v) { v.x; v.y; v.z; v.w; } && !std::derived_from<std::remove_cvref_t<U>, vec_tag>)
        vec4(const U& vec_type);
        vec4(T x, T y, T z, T w);
        vec4(T s);

        T x;
        T y;
        T z;
        T w;
    });

    ABY_RHI_PACKED(struct mat4 {
        /// @brief Your mat4 type must be 16 contiguous floats only ie. glm::mat4 
        template <typename T>
        mat4(const T& mat_type) {
            std::memcpy(data, reinterpret_cast<float*>(*mat_type), sizeof(float) * 16);
        }

        float data[16];
    });

    template <typename T>
    template <typename U> requires ( requires(const U& v) { v.x; v.y; } && !std::derived_from<std::remove_cvref_t<U>, vec_tag>)
    vec2<T>::vec2(const U& vec_type) :
        x(static_cast<T>(vec_type.x)),
        y(static_cast<T>(vec_type.y)) {}

    template <typename T>
    vec2<T>::vec2(T x, T y) : x(x), y(y) {}

    template <typename T>
    vec2<T>::vec2(T s) : x(s), y(s) {}

    template <typename T>
    template <typename U> requires (requires(const U& v) { v.x; v.y; v.z; } && !std::derived_from<std::remove_cvref_t<U>, vec_tag>)
    vec3<T>::vec3(const U& vec_type) : 
        x(static_cast<T>(vec_type.x)),
        y(static_cast<T>(vec_type.y)),
        z(static_cast<T>(vec_type.z)) {}

    template <typename T>
    vec3<T>::vec3(T x, T y, T z) : x(x), y(y), z(z) {}

    template <typename T>
    vec3<T>::vec3(T s) : x(s), y(s), z(s) {}

    template <typename T>
    template <typename U> requires ( requires(const U& v) { v.x; v.y; v.z; v.w; } && !std::derived_from<std::remove_cvref_t<U>, vec_tag>)
    vec4<T>::vec4(const U& vec_type) :
        x(static_cast<T>(vec_type.x)),
        y(static_cast<T>(vec_type.y)),
        z(static_cast<T>(vec_type.z)),
        w(static_cast<T>(vec_type.w)) {}

    template <typename T>
    vec4<T>::vec4(T x, T y, T z, T w) : x(x), y(y), z(z), w(w) {}

    template <typename T>
    vec4<T>::vec4(T s) : x(s), y(s), z(s), w(s) {}

}