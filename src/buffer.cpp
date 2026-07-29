#include "buffer.hpp"
#include "context.hpp"
#include "backends/vulkan/vulkan-buffer.hpp"

namespace aby::rhi {

    Buffer::Buffer(size_t size, size_t stride) :
        m_Size(size),
        m_Stride(stride),
        m_Count(0)
    {
        m_Data.resize(size);
    }

    auto Buffer::stride() const -> size_t {
        return m_Stride;
    }

    auto Buffer::capacity_bytes() const -> size_t {
        return m_Size;
    }

    auto Buffer::used_bytes() const -> size_t {
        return m_Count * m_Stride;
    }

    auto Buffer::count() const -> size_t {
        return m_Count;
    }

    auto Buffer::clear() -> void {
        m_Count = 0;
    }

    auto VertexBuffer::create(size_t size, size_t stride) -> std::shared_ptr<VertexBuffer> {
        auto& ctx = Context::get();
        switch (ctx.renderer_backend()) {
            case ERenderer::vulkan: 
                return std::make_shared<vulkan::VertexBuffer>(size, stride);
            default:
                aby_rhi_assert(false, "VertexBuffer for renderer backend: {} not implemented", ctx.renderer_backend());
        }
        return nullptr;
    }
 
    VertexBuffer::VertexBuffer(size_t size, size_t stride) :
        Buffer(size, stride)
    {

    }

    auto IndexBuffer::create(size_t size) -> std::shared_ptr<IndexBuffer> {
        auto& ctx = Context::get();
        switch (ctx.renderer_backend()) {
            case ERenderer::vulkan: 
                return std::make_shared<vulkan::IndexBuffer>(size);
            default:
                aby_rhi_assert(false, "IndexBuffer for renderer backend: {} not implemented", ctx.renderer_backend());
        }
        return nullptr;
    }

    IndexBuffer::IndexBuffer(size_t size) :
        Buffer(size, sizeof(uint32_t))
    {

    }

    auto IndexBuffer::push(uint32_t index) -> void {
        aby_rhi_assert(used_bytes() + sizeof(uint32_t) <= capacity_bytes(), "VertexBuffer out of range");
        std::memcpy(m_Data.data() + (m_Count * sizeof(uint32_t)), &index, sizeof(uint32_t));
        m_Count++;
    }



}