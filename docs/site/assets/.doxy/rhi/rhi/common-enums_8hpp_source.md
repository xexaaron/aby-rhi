

# File common-enums.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**common-enums.hpp**](common-enums_8hpp.md)

[Go to the documentation of this file](common-enums_8hpp.md)


```C++
#pragma once
#include <cstdint>
#include <format>
#include <string>

namespace aby::rhi {

    enum class EAllocation {
        backend,
    };

    enum class ELogLevel {
        debug,
        trace,
        info,
        warn,
        error,
        fatal,
    };

    enum class ERenderer {
        vulkan,
    };

    enum class EWindow {
        win32,   // windows
        x11,     // linux
        xcb,     // linux
        wayland, // linux
        metal    // macos
    };

    enum class EShader {
        none = 0,
        vert = 1,
        frag = 2,
        px   = frag,
        comp = 3,
        geom = 4,
    };

    enum class ETopology {
        point_list,
        line_list,
        line_strip,
        triangle_list,
        triangle_strip,
        triangle_fan,
    };

    enum class EPolygonMode {
        fill,
        line,
        point,
    };

    enum class ECullMode {
        none,
        front,
        back,
        front_and_back,
    };

    enum class EFrontFace {
        clockwise,
        counter_clockwise,
    };

    enum class EFormat {
        none,

        rgba_f32,
        rgb_f32,
        rg_f32,
        r_f32,

        rgba_f16,
        rgb_f16,
        rg_f16,
        r_f16,

        rgba_i32,
        rgb_i32,
        rg_i32,
        r_i32,

        rgba_u32,
        rgb_u32,
        rg_u32,
        r_u32,
    };

    enum class EJobPriority {
        low      = 0,
        medium   = 1,
        high     = 2,
        critical = 3
    };

    enum class EJobQueue {
        textures = 0,
        shaders  = 1,
        caching  = 2,
        max_queues
    };

    enum class EResource : uint32_t {
        none,
        texture,
        shader,
    };

    enum class EResourceState : uint16_t {
        invalid,
        loading,
        loaded,
        failed,
    };

    enum class ResourceID : uint32_t {
        invalid = UINT32_MAX,
    };

    enum class EFiltering {
        linear,
        nearest,
        cubic
    };

    enum class ERepeatMode {
        repeat,
        mirrored,
        clamp_to_edge,
        clamp_to_border,
        mirrored_clamp_to_edge,
    };

    enum class EAntiAliasing {
        none,
        msaa2x,
        msaa4x,
        msaa8x
    };

    enum class ECompareOp {
        never,
        less,
        eq,
        less_eq,
        greater,
        neq,
        greater_eq,
        always
    };

    enum class EBlendFactor {
        zero,
        one,
        src_color,
        one_minus_src_color,
        dst_color,
        one_minus_dst_color,
        src_alpha,
        one_minus_src_alpha,
        dst_alpha,
        one_minus_dst_alpha,
        constant_color,
        one_minus_constant_color,
        constant_alpha,
        one_minus_constant_alpha,
        src_alpha_saturate,
        src_one_color,
        one_minus_src_one_color,
        src_one_alpha,
        one_minus_src_one_alpha,
    };

    enum class EBlendOp {
        add,
        sub,
        reverse_sub,
        min,
        max,
    };

    enum class ETextureUsage {
        albedo,   // srgb format for base color.
        material, // unorm format, for normals, etc...
    };

    enum class EChannels : uint8_t {
        none = 0,

        r = 1 << 0,
        g = 1 << 1,
        b = 1 << 2,
        a = 1 << 3,

        rg = r | g,
        rb = r | b,
        ra = r | a,

        gb = g | b,
        ga = g | a,
        ba = b | a,

        rgb = r | g | b,
        rga = r | g | a,
        rba = r | b | a,
        gba = g | b | a,

        rgba = r | g | b | a
    };

} // namespace aby::rhi

namespace aby::rhi {

    constexpr aby::rhi::EChannels operator|(EChannels lhs, EChannels rhs) {
        return static_cast<EChannels>(static_cast<uint8_t>(lhs) | static_cast<uint8_t>(rhs));
    }

    constexpr EChannels operator&(EChannels lhs, EChannels rhs) {
        return static_cast<EChannels>(static_cast<uint8_t>(lhs) & static_cast<uint8_t>(rhs));
    }

} // namespace aby::rhi

namespace std {

    template <>
    struct formatter<aby::rhi::EAllocation, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EAllocation value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::EAllocation::backend:
                    str = "backend";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::EShader, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EShader value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::EShader::none:
                    str = "none";
                    break;
                case aby::rhi::EShader::vert:
                    str = "vert";
                    break;
                case aby::rhi::EShader::frag:
                    str = "frag";
                    break;
                case aby::rhi::EShader::comp:
                    str = "comp";
                    break;
                case aby::rhi::EShader::geom:
                    str = "geom";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::ELogLevel, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::ELogLevel value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::ELogLevel::debug:
                    str = "debug";
                    break;
                case aby::rhi::ELogLevel::trace:
                    str = "trace";
                    break;
                case aby::rhi::ELogLevel::info:
                    str = "info";
                    break;
                case aby::rhi::ELogLevel::warn:
                    str = "warn";
                    break;
                case aby::rhi::ELogLevel::error:
                    str = "error";
                    break;
                case aby::rhi::ELogLevel::fatal:
                    str = "fatal";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::ERenderer, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::ERenderer value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::ERenderer::vulkan:
                    str = "vulkan";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::EWindow, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EWindow value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::EWindow::win32:
                    str = "win32";
                    break;
                case aby::rhi::EWindow::x11:
                    str = "x11";
                    break;
                case aby::rhi::EWindow::xcb:
                    str = "xcb";
                    break;
                case aby::rhi::EWindow::wayland:
                    str = "wayland";
                    break;
                case aby::rhi::EWindow::metal:
                    str = "metal";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::ETopology, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::ETopology value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::ETopology::point_list:
                    str = "point_list";
                    break;
                case aby::rhi::ETopology::line_list:
                    str = "line_list";
                    break;
                case aby::rhi::ETopology::line_strip:
                    str = "line_strip";
                    break;
                case aby::rhi::ETopology::triangle_list:
                    str = "triangle_list";
                    break;
                case aby::rhi::ETopology::triangle_strip:
                    str = "triangle_strip";
                    break;
                case aby::rhi::ETopology::triangle_fan:
                    str = "triangle_fan";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::EPolygonMode, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EPolygonMode value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::EPolygonMode::fill:
                    str = "fill";
                    break;
                case aby::rhi::EPolygonMode::line:
                    str = "line";
                    break;
                case aby::rhi::EPolygonMode::point:
                    str = "point";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::ECullMode, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::ECullMode value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::ECullMode::none:
                    str = "none";
                    break;
                case aby::rhi::ECullMode::front:
                    str = "front";
                    break;
                case aby::rhi::ECullMode::back:
                    str = "back";
                    break;
                case aby::rhi::ECullMode::front_and_back:
                    str = "front_and_back";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::EFrontFace, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EFrontFace value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::EFrontFace::clockwise:
                    str = "clockwise";
                    break;
                case aby::rhi::EFrontFace::counter_clockwise:
                    str = "counter_clockwise";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::EFormat, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EFormat value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::EFormat::none:
                    str = "none";
                    break;
                case aby::rhi::EFormat::rgba_f32:
                    str = "float[4]";
                    break;
                case aby::rhi::EFormat::rgb_f32:
                    str = "float[3]";
                    break;
                case aby::rhi::EFormat::rg_f32:
                    str = "float[2]";
                    break;
                case aby::rhi::EFormat::r_f32:
                    str = "float";
                    break;
                case aby::rhi::EFormat::rgba_i32:
                    str = "int32[4]";
                    break;
                case aby::rhi::EFormat::rgb_i32:
                    str = "int32[3]";
                    break;
                case aby::rhi::EFormat::rg_i32:
                    str = "int32[2]";
                    break;
                case aby::rhi::EFormat::r_i32:
                    str = "int32";
                    break;
                case aby::rhi::EFormat::rgba_u32:
                    str = "uint32[4]";
                    break;
                case aby::rhi::EFormat::rgb_u32:
                    str = "uint32[3]";
                    break;
                case aby::rhi::EFormat::rg_u32:
                    str = "uint32[2]";
                    break;
                case aby::rhi::EFormat::r_u32:
                    str = "uint32";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::EJobPriority, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EJobPriority value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::EJobPriority::low:
                    str = "low";
                    break;
                case aby::rhi::EJobPriority::medium:
                    str = "medium";
                    break;
                case aby::rhi::EJobPriority::high:
                    str = "high";
                    break;
                case aby::rhi::EJobPriority::critical:
                    str = "critical";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::EJobQueue, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EJobQueue value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::EJobQueue::textures:
                    str = "textures";
                    break;
                case aby::rhi::EJobQueue::shaders:
                    str = "shaders";
                    break;
                case aby::rhi::EJobQueue::caching:
                    str = "caching";
                    break;
                case aby::rhi::EJobQueue::max_queues:
                    str = "max queues";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::EResource, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EResource value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::EResource::none:
                    str = "none";
                    break;
                case aby::rhi::EResource::texture:
                    str = "texture";
                    break;
                case aby::rhi::EResource::shader:
                    str = "shader";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::EResourceState, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EResourceState value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::EResourceState::invalid:
                    str = "invalid";
                    break;
                case aby::rhi::EResourceState::loading:
                    str = "loading";
                    break;
                case aby::rhi::EResourceState::loaded:
                    str = "loaded";
                    break;
                case aby::rhi::EResourceState::failed:
                    str = "failed";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::ResourceID, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::ResourceID value, FmtContext& ctx) const {
            switch (value) {
                case aby::rhi::ResourceID::invalid:
                    return std::format_to(ctx.out(), "<invalid>");

                default:
                    return std::format_to(
                        ctx.out(),
                        "<{}>",
                        std::to_underlying(value));
            }
        }
    };

    template <>
    struct formatter<aby::rhi::EFiltering, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EFiltering value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::EFiltering::linear:
                    str = "linear";
                    break;
                case aby::rhi::EFiltering::nearest:
                    str = "nearest";
                    break;
                case aby::rhi::EFiltering::cubic:
                    str = "cubic";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::ERepeatMode, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::ERepeatMode value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";
            switch (value) {
                case aby::rhi::ERepeatMode::repeat:
                    str = "repeat";
                    break;
                case aby::rhi::ERepeatMode::mirrored:
                    str = "mirrored";
                    break;
                case aby::rhi::ERepeatMode::clamp_to_edge:
                    str = "clamp to edge";
                    break;
                case aby::rhi::ERepeatMode::clamp_to_border:
                    str = "clamp to border";
                    break;
                case aby::rhi::ERepeatMode::mirrored_clamp_to_edge:
                    str = "mirrored clamp to edge";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::EAntiAliasing, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EAntiAliasing value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";
            switch (value) {
                case aby::rhi::EAntiAliasing::none:
                    str = "none";
                    break;
                case aby::rhi::EAntiAliasing::msaa2x:
                    str = "msaa 2x";
                    break;
                case aby::rhi::EAntiAliasing::msaa4x:
                    str = "msaa 4x";
                    break;
                case aby::rhi::EAntiAliasing::msaa8x:
                    str = "msaa 8x";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::ECompareOp, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::ECompareOp value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";
            switch (value) {
                case aby::rhi::ECompareOp::never:
                    str = "never";
                    break;
                case aby::rhi::ECompareOp::less:
                    str = "less";
                    break;
                case aby::rhi::ECompareOp::eq:
                    str = "eq";
                    break;
                case aby::rhi::ECompareOp::less_eq:
                    str = "less_eq";
                    break;
                case aby::rhi::ECompareOp::greater:
                    str = "greater";
                    break;
                case aby::rhi::ECompareOp::neq:
                    str = "neq";
                    break;
                case aby::rhi::ECompareOp::greater_eq:
                    str = "greater_eq";
                    break;
                case aby::rhi::ECompareOp::always:
                    str = "always";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::EBlendFactor, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EBlendFactor value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";
            switch (value) {
                case aby::rhi::EBlendFactor::zero:
                    str = "zero";
                    break;
                case aby::rhi::EBlendFactor::one:
                    str = "one";
                    break;
                case aby::rhi::EBlendFactor::src_color:
                    str = "src_color";
                    break;
                case aby::rhi::EBlendFactor::one_minus_src_color:
                    str = "one_minus_src_color";
                    break;
                case aby::rhi::EBlendFactor::dst_color:
                    str = "dst_color";
                    break;
                case aby::rhi::EBlendFactor::one_minus_dst_color:
                    str = "one_minus_dst_color";
                    break;
                case aby::rhi::EBlendFactor::src_alpha:
                    str = "src_alpha";
                    break;
                case aby::rhi::EBlendFactor::one_minus_src_alpha:
                    str = "one_minus_src_alpha";
                    break;
                case aby::rhi::EBlendFactor::dst_alpha:
                    str = "dst_alpha";
                    break;
                case aby::rhi::EBlendFactor::one_minus_dst_alpha:
                    str = "one_minus_dst_alpha";
                    break;
                case aby::rhi::EBlendFactor::constant_color:
                    str = "constant_color";
                    break;
                case aby::rhi::EBlendFactor::one_minus_constant_color:
                    str = "one_minus_constant_color";
                    break;
                case aby::rhi::EBlendFactor::constant_alpha:
                    str = "constant_alpha";
                    break;
                case aby::rhi::EBlendFactor::one_minus_constant_alpha:
                    str = "one_minus_constant_alpha";
                    break;
                case aby::rhi::EBlendFactor::src_alpha_saturate:
                    str = "src_alpha_saturate";
                    break;
                case aby::rhi::EBlendFactor::src_one_color:
                    str = "src_one_color";
                    break;
                case aby::rhi::EBlendFactor::one_minus_src_one_color:
                    str = "one_minus_src_one_color";
                    break;
                case aby::rhi::EBlendFactor::src_one_alpha:
                    str = "src_one_alpha";
                    break;
                case aby::rhi::EBlendFactor::one_minus_src_one_alpha:
                    str = "one_minus_src_one_alpha";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::EBlendOp, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EBlendOp value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";
            switch (value) {
                case aby::rhi::EBlendOp::add:
                    str = "add";
                    break;
                case aby::rhi::EBlendOp::sub:
                    str = "sub";
                    break;
                case aby::rhi::EBlendOp::reverse_sub:
                    str = "reverse_sub";
                    break;
                case aby::rhi::EBlendOp::min:
                    str = "min";
                    break;
                case aby::rhi::EBlendOp::max:
                    str = "max";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::ETextureUsage, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::ETextureUsage value, FmtContext& ctx) const {
            std::string_view str = "<unknown>";

            switch (value) {
                case aby::rhi::ETextureUsage::albedo:
                    str = "albedo";
                    break;
                case aby::rhi::ETextureUsage::material:
                    str = "material";
                    break;
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

    template <>
    struct formatter<aby::rhi::EChannels, char> {
        template <class ParseContext>
        constexpr ParseContext::iterator parse(ParseContext& ctx) {
            auto it = ctx.begin();
            if (it != ctx.end() && *it != '}')
                throw format_error("invalid format");
            return it;
        }

        template <class FmtContext>
        FmtContext::iterator format(aby::rhi::EChannels value, FmtContext& ctx) const {
            std::string str = "";

            if ((value & aby::rhi::EChannels::r) != aby::rhi::EChannels::none) {
                str += "r";
            }
            if ((value & aby::rhi::EChannels::g) != aby::rhi::EChannels::none) {
                str += "g";
            }
            if ((value & aby::rhi::EChannels::b) != aby::rhi::EChannels::none) {
                str += "b";
            }
            if ((value & aby::rhi::EChannels::a) != aby::rhi::EChannels::none) {
                str += "a";
            }

            if (str.empty()) {
                str = "<unknown>";
            }

            return std::ranges::copy(str, ctx.out()).out;
        }
    };

} // namespace std
```


