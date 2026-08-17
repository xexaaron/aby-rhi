

# File vulkan-gc.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**backends**](dir_b2e089009e3d6019946b366985df1d18.md) **>** [**vulkan**](dir_8ecac3f6860031a9c88024a6c0dd2404.md) **>** [**vulkan-gc.hpp**](vulkan-gc_8hpp.md)

[Go to the documentation of this file](vulkan-gc_8hpp.md)


```C++
#pragma once
#include "common.hpp"

#include <functional>
#include <stack>

namespace aby::rhi::vulkan {

    class GarbageCollector {
    public:
        using Dtor = std::function<void()>;

        auto push(Dtor&& dtor) -> void;
        auto run() -> void;
    private:
        std::stack<Dtor> m_Dtors;
    };

}; // namespace aby::rhi::vulkan
```


