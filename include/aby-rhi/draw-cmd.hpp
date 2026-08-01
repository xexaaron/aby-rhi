#pragma once
#include "common.hpp"
#include "buffer.hpp"

namespace aby::rhi {

    /**
     * @brief Draw Cmd ares to be submitted to a created render pass each frame.
     */
    class DrawCmd {
    public:
        /**
         * @brief DrawCmd constructor
         * @param vertices A vertex buffer
         * @param indices  An index buffer
         * @param instances The amount of times the same set of vertices and indices is to be drawn
         */
        DrawCmd(std::shared_ptr<VertexBuffer> vertices, std::shared_ptr<IndexBuffer> indices, uint32_t instances = 1);
        /**
         * @brief Set the instance count
         * @param instance_count The amount of times the same set of vertices and indices is to be drawn
         */
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