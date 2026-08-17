

# File draw-cmd.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**draw-cmd.hpp**](draw-cmd_8hpp.md)

[Go to the documentation of this file](draw-cmd_8hpp.md)


```C++
#pragma once
#include "buffer.hpp"
#include "common.hpp"

namespace aby::rhi {

    class DrawCmd {
    public:
        DrawCmd(std::shared_ptr<VertexBuffer> vertices, std::shared_ptr<IndexBuffer> indices, uint32_t instances = 1);
        auto set_instance_count(uint32_t instance_count) -> void;
        auto vbuff() const -> VertexBuffer*;
        auto ibuff() const -> IndexBuffer*;
        auto instances() const -> uint32_t;
    private:
        VertexBuffer* m_VertexBuffer;
        IndexBuffer* m_IndexBuffer;
        uint32_t m_InstanceCount;
    };

} // namespace aby::rhi
```


