#pragma once
#include "common.hpp"
#include "buffer.hpp"

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
        IndexBuffer*  m_IndexBuffer;
        uint32_t      m_InstanceCount;
    };

}