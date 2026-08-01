#include "draw-cmd.hpp"

namespace aby::rhi {

	DrawCmd::DrawCmd(std::shared_ptr<VertexBuffer> vertices, std::shared_ptr<IndexBuffer> indices, uint32_t instances) :
	    m_VertexBuffer(vertices.get()),
	    m_IndexBuffer(indices.get()),
	    m_InstanceCount(instances) {
	}

	auto DrawCmd::set_instance_count(uint32_t instance_count) -> void {
		m_InstanceCount = instance_count;
	}

	auto DrawCmd::vbuff() const -> VertexBuffer* {
		return m_VertexBuffer;
	}

	auto DrawCmd::ibuff() const -> IndexBuffer* {
		return m_IndexBuffer;
	}

	auto DrawCmd::instances() const -> uint32_t {
		return m_InstanceCount;
	}

} // namespace aby::rhi
