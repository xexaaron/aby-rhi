

# File buffer.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**buffer.hpp**](buffer_8hpp.md)

[Go to the documentation of this file](buffer_8hpp.md)


```C++
#pragma once
#include "common.hpp"

#include <cassert>
#include <memory>
#include <vector>

namespace aby::rhi {

    class Buffer {
    public:
        Buffer(size_t size, size_t stride);
        virtual ~Buffer() = default;

        virtual auto upload() -> void  = 0;
        virtual auto destroy() -> void = 0;
        auto stride() const -> size_t;
        auto capacity_bytes() const -> size_t;
        auto used_bytes() const -> size_t;
        auto count() const -> size_t;
        auto clear() -> void;
    protected:
        size_t m_Stride;
        size_t m_Size;
        size_t m_Count;
        uint8_t* m_Data;
    };

    class VertexBuffer : public Buffer {
    public:
        static auto create(size_t size, size_t stride) -> std::shared_ptr<VertexBuffer>;
        VertexBuffer(size_t size, size_t stride);
        virtual auto upload() -> void  = 0;
        virtual auto destroy() -> void = 0;
        auto push(void* v) -> void;
    private:
    };

    class IndexBuffer : public Buffer {
    public:
        static auto create(size_t size) -> std::shared_ptr<IndexBuffer>;
        IndexBuffer(size_t size);
        virtual auto upload() -> void  = 0;
        virtual auto destroy() -> void = 0;
        auto push(uint32_t index) -> void;
    private:
    };

} // namespace aby::rhi
```


