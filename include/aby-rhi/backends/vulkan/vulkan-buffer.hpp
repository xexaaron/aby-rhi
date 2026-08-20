#pragma once
#include "buffer.hpp"

#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

	class Buffer {
	public:
		Buffer();
		Buffer(size_t size, vk::BufferUsageFlags usage, VmaMemoryUsage memory_usage);
		Buffer(const Buffer& other);
		Buffer(Buffer&& other);

		~Buffer();

		auto clear() -> void;
		auto copy_to(vk::Buffer dst, size_t bytes) -> bool;
		auto write(const void* data, size_t bytes) -> void;
		auto read(std::vector<uint8_t>* out_data) -> void;
		auto allocation() -> VmaAllocation;
		auto allocation_info() -> VmaAllocationInfo;
		auto destroy() -> void;

		Buffer& operator=(Buffer&& other);

		operator VkBuffer();
		operator vk::Buffer();
		operator vk::DeviceAddress();
	private:
		vk::Buffer m_Buffer;
		vk::DeviceAddress m_Address;
		VmaAllocation m_Alloc;
		VmaAllocationInfo m_AllocInfo;
	};

	class VertexBuffer : public rhi::VertexBuffer {
	public:
		VertexBuffer(size_t size, size_t stride);

		auto upload() -> void override;
		auto destroy() -> void override;
		auto gpu() -> vulkan::Buffer&;
	private:
		vulkan::Buffer m_GPUData;
	};

	class IndexBuffer : public rhi::IndexBuffer {
	public:
		IndexBuffer(size_t size);

		auto upload() -> void override;
		auto destroy() -> void override;
		auto gpu() -> vulkan::Buffer&;
	private:
		vulkan::Buffer m_GPUData;
	};

} // namespace aby::rhi::vulkan
