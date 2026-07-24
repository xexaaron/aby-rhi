#pragma once

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

}