#pragma once

namespace aby::rhi {

    enum class EAllocation {
        backend,
        interface,
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
        rgba_sf16,
    };

}