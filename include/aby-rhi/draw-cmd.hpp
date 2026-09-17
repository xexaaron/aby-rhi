#pragma once
#include "buffer.hpp"
#include "common.hpp"

namespace aby::rhi {

	/**
     * @brief Draw Cmd ares to be submitted to a created render pass each frame.
     */
	class ABY_RHI_API DrawCmd {
	public:
		/**
         * @brief DrawCmd constructor
         * @param[in] vertices A vertex buffer
         * @param[in] indices  An index buffer
         * @param[in] instances The amount of times the same set of vertices and indices is to be drawn
         */
		DrawCmd(std::shared_ptr<VertexBuffer> vertices, std::shared_ptr<IndexBuffer> indices, uint32_t instances = 1);
		/**
         * @brief Set the instance count
         * @param[in] instance_count The amount of times the same set of vertices and indices is to be drawn
         */
		auto set_instance_count(uint32_t instance_count) -> void;
		/// @brief Get the vertex buffer
		auto vbuff() const -> VertexBuffer*;
		/// @brief Get the index buffer
		auto ibuff() const -> IndexBuffer*;
		/// @brief Get the instance count
		auto instances() const -> uint32_t;
	private:
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;
		uint32_t m_InstanceCount;
	};

} // namespace aby::rhi
