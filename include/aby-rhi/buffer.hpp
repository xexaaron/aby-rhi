#pragma once
#include "common.hpp"

#include <cassert>
#include <memory>
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
         * @brief Create a vertex buffer
         * @param size The desired vertex count of the buffer
         * @param stride The byte size of each vertex in the buffer.
        */
		static auto create(size_t size, size_t stride) -> std::shared_ptr<VertexBuffer>;
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
		auto push(void* v) -> void;
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
         * @param v An index
        */
		auto push(uint32_t index) -> void;
	private:
	};

} // namespace aby::rhi
