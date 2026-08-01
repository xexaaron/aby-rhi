#include "buffer.hpp"

#include "backends/vulkan/vulkan-buffer.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"
#include "context.hpp"

namespace aby::rhi {

	Buffer::Buffer(size_t size, size_t stride) :
	    m_Size(size * stride),
	    m_Stride(stride),
	    m_Count(0) {
		m_Data.resize(m_Size);
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
			case ERenderer::vulkan: {
				auto ptr = std::make_shared<vulkan::VertexBuffer>(size, stride);
				auto* r  = static_cast<vulkan::Renderer*>(ctx.renderer());
				r->gc().push([weak = std::weak_ptr(ptr)] {
					if (auto p = weak.lock())
						p->destroy();
				});
				return ptr;
			}
			default:
				aby_rhi_assert(false, "VertexBuffer for renderer backend: {} not implemented", ctx.renderer_backend());
		}
		return nullptr;
	}

	VertexBuffer::VertexBuffer(size_t size, size_t stride) :
	    Buffer(size, stride) {
	}

	auto VertexBuffer::push(void* v) -> void {
		aby_rhi_assert(used_bytes() + m_Stride <= capacity_bytes(),
		               "VertexBuffer is full: {} + {} <= {}",
		               used_bytes(), m_Stride, capacity_bytes());
		std::memcpy(m_Data.data() + (m_Count * m_Stride), v, m_Stride);
		m_Count++;
	}

	auto IndexBuffer::create(size_t size) -> std::shared_ptr<IndexBuffer> {
		auto& ctx = Context::get();
		switch (ctx.renderer_backend()) {
			case ERenderer::vulkan: {
				auto ptr = std::make_shared<vulkan::IndexBuffer>(size);
				auto* r  = static_cast<vulkan::Renderer*>(ctx.renderer());
				r->gc().push([weak = std::weak_ptr(ptr)] {
					if (auto p = weak.lock())
						p->destroy();
				});
				return ptr;
			}

			default:
				aby_rhi_assert(false, "IndexBuffer for renderer backend: {} not implemented", ctx.renderer_backend());
		}
		return nullptr;
	}

	IndexBuffer::IndexBuffer(size_t size) :
	    Buffer(size, sizeof(uint32_t)) {
	}

	auto IndexBuffer::push(uint32_t index) -> void {
		aby_rhi_assert(used_bytes() + sizeof(uint32_t) <= capacity_bytes(), "VertexBuffer out of range");
		std::memcpy(m_Data.data() + (m_Count * sizeof(uint32_t)), &index, sizeof(uint32_t));
		m_Count++;
	}

} // namespace aby::rhi
