#pragma once
#include "common.hpp"

#include <cassert>
#include <memory>
#include <ranges>
#include <vector>

namespace aby::rhi {

	class Buffer {
	public:
		/**
         * @brief Buffer constructor
         * @param size The desired element count of the buffer
         * @param stride The byte size of each element in the buffer.
        */
		Buffer(size_t size, size_t stride);
		virtual ~Buffer() = default;

		/**
         * @brief Upload the buffers cpu data to the GPU
        */
		virtual auto upload() -> void  = 0;
		/**
         * @brief Cleanup all resources
        */
		virtual auto destroy() -> void = 0;
		/**
         * @brief The byte size of each element
        */
		auto stride() const -> size_t;
		/**
         * @brief The total amount of bytes
         * @return stride() * count()
        */
		auto capacity_bytes() const -> size_t;
		/**
         * @brief The amount of bytes already pushed into the buffer.
        */
		auto used_bytes() const -> size_t;
		/**
         * @brief The number of elements in the buffer
         * @return used_bytes() / stride() 
        */
		auto count() const -> size_t;
		/**
         * @brief Clears all data from the buffer (CPU ONLY)
        */
		auto clear() -> void;
	protected:
		size_t m_Stride;
		size_t m_Size;
		size_t m_Count;
		uint8_t* m_Data;
	};

	class VertexBuffer : public Buffer {
	public:
		/**
         * @brief Create a vertex buffer.
         * @param size The desired vertex count of the buffer.
         * @param stride The byte size of each vertex in the buffer.
        */
		static auto create(size_t size, size_t stride) -> std::shared_ptr<VertexBuffer>;
		/**
		 * @brief Create a vertex buffer.
		 * @tparam T the type of the vertex.
		 * @param size The desired vertex count of the buffer.
		 */
		template <typename T>
		static auto create(size_t size) -> std::shared_ptr<VertexBuffer> {
			return create(size, sizeof(T));
		}
		/**
         * @brief VertexBuffer constructor
         * @param size The desired vertex count of the buffer
         * @param stride The byte size of each vertex in the buffer.
        */
		VertexBuffer(size_t size, size_t stride);
		/**
         * @brief Upload the buffers cpu data to the GPU
        */
		virtual auto upload() -> void  = 0;
		/**
         * @brief Cleanup all resources
        */
		virtual auto destroy() -> void = 0;
		/**
         * @brief Push a vertex into the cpu buffer
         * @param v A vertex pointer matching the stride set by the constructor.
        */
		auto push(const void* vertex) -> void;
		/**
		 * @brief Push a vertex into the CPU buffer.
		 * @tparam T Vertex type. (must match the stride set by the constructor)
		 * @param vertex The vertex to push.
		*/
		template <typename T>
		auto push(const T& vertex) -> void {
			push(&vertex);
		}
		/**
		 * @brief Push an array of vertices into the cp u buffer
		 * @param vertices an array of vertices
		 * @param count the number of vertices in the array
		 */
		auto push(const void* vertices, size_t count) -> void;
		/**
		 * @brief Push a range of vertices into the CPU buffer.
		 * @tparam Range a contiguous range of vertices.
		 * @param range the range of vertices to push.
		 */
		template <std::ranges::contiguous_range Range>
		auto push(const Range& range) -> void {
			push(std::ranges::data(range), std::ranges::size(range));
		}
	private:
	};

	class IndexBuffer : public Buffer {
	public:
		/**
         * @brief Create an Index buffer
         * @param size The desired index count of the buffer (uint32_t indices)
        */
		static auto create(size_t size) -> std::shared_ptr<IndexBuffer>;
		/**
         * @brief IndexBuffer constructor
         * @param size The desired index count of the buffer (uint32_t indices)
        */
		IndexBuffer(size_t size);
		/**
         * @brief Upload the buffers cpu data to the GPU
        */
		virtual auto upload() -> void  = 0;
		/**
         * @brief Cleanup all resources
        */
		virtual auto destroy() -> void = 0;
		/**
         * @brief Push an index into the cpu buffer
         * @param index An index
        */
		auto push(uint32_t index) -> void;
		/**
		 * @brief Push an array of indices into the cpu buffer
		 * @param indices an view of indices
		 */
		auto push(std::span<const uint32_t> indices) -> void;
		/**
		 * @brief Push a range of indices into the cpu buffer
		 * @param Range a contiguous range of vertices
		 * @param indices the range of indices to push.
		 */
		template <std::ranges::contiguous_range Range>
		auto push(const Range& indices) -> void {
			push(std::span<const uint32_t>(std::ranges::data(indices), std::ranges::size(indices)));
		}
	private:
	};

} // namespace aby::rhi
