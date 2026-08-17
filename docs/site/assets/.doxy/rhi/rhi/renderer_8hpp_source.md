

# File renderer.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**renderer.hpp**](renderer_8hpp.md)

[Go to the documentation of this file](renderer_8hpp.md)


```C++
#pragma once
#include "common.hpp"
#include "render-pass.hpp"

#include <cstdint>

namespace aby::rhi {

    struct GraphicsParams {
    };

    class Renderer {
    public:
        static auto create(ERenderer backend, const GraphicsParams& graphics) -> Renderer*;
        virtual ~Renderer() = default;

        virtual auto init(void* native_window) -> bool                         = 0;
        virtual auto deinit() -> void                                          = 0;
        virtual auto add_pass(std::shared_ptr<RenderPass> render_pass) -> void = 0;
        virtual auto set_clear_color(Color color) -> void                      = 0;
        virtual auto on_begin() -> bool                                        = 0;
        virtual auto on_end() -> bool                                          = 0;
    private:
    };

} // namespace aby::rhi
```


