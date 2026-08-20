#include "backends/vulkan/vulkan-buffer.hpp"

#include "backends/vulkan/vulkan-common.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"

namespace aby::rhi::vulkan {

	Buffer::Buffer() :
	    m_Alloc(VK_NULL_HANDLE),
	    m_AllocInfo{},
	    m_Address(0),
	    m_Buffer{} {
	}

	Buffer::Buffer(size_t size, vk::BufferUsageFlags usage, VmaMemoryUsage memory_usage) {
		vk::BufferCreateInfo create_info(
		    vk::BufferCreateFlags(),
		    size,
		    usage);

		VmaAllocationCreateFlags alloc_flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;
		if (memory_usage == VMA_MEMORY_USAGE_AUTO) {
			alloc_flags |= VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
		}

		VmaAllocationCreateInfo alloc_create_info = {};
		alloc_create_info.usage                   = memory_usage;
		alloc_create_info.flags                   = alloc_flags;

		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		vkassert(vmaCreateBuffer(
		             r->vma(),
		             vkcast(create_info),
		             &alloc_create_info,
		             vkcast(m_Buffer),
		             &m_Alloc,
		             &m_AllocInfo),
		         "failed to create VMA buffer");

		if (usage & vk::BufferUsageFlagBits::eShaderDeviceAddress) {
			vk::BufferDeviceAddressInfo info(m_Buffer);
			m_Address = vkGetBufferDeviceAddress(r->device(), vkcast(info));
		}
	}

	Buffer::Buffer(const Buffer& other) :
	    m_Buffer(other.m_Buffer),
	    m_Address(other.m_Address),
	    m_Alloc(other.m_Alloc),
	    m_AllocInfo(other.m_AllocInfo) {
	}

	Buffer::Buffer(Buffer&& other) :
	    m_Buffer(other.m_Buffer),
	    m_Address(other.m_Address),
	    m_Alloc(other.m_Alloc),
	    m_AllocInfo(other.m_AllocInfo) {
		other.m_Buffer    = VK_NULL_HANDLE;
		other.m_Address   = 0;
		other.m_Alloc     = VK_NULL_HANDLE;
		other.m_AllocInfo = {};
	}

	Buffer::~Buffer() {
		destroy();
	}

	auto Buffer::clear() -> void {
		std::memset(m_AllocInfo.pMappedData, 0, m_AllocInfo.size);
	}

	auto Buffer::write(const void* data, size_t bytes) -> void {
		std::memcpy(m_AllocInfo.pMappedData, data, bytes);
	}

	auto Buffer::read(std::vector<uint8_t>* out_data) -> void {
		uint8_t* bytes = static_cast<uint8_t*>(m_AllocInfo.pMappedData);
		out_data->assign(bytes, bytes + m_AllocInfo.size);
	}

	auto Buffer::copy_to(vk::Buffer dst, size_t bytes) -> bool {
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		if (!r->immediate_submit([&](vk::CommandBuffer cmd) {
			vk::BufferCopy copy(0, 0, bytes);
			vkCmdCopyBuffer(
			    cmd,
			    this->m_Buffer,
			    dst,
			    1,
			    vkcast(copy));
		})) {
			return false;
		}
		return true;
	}

	auto Buffer::destroy() -> void {
		if (m_Buffer) {
			auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

			vmaDestroyBuffer(
			    r->vma(),
			    m_Buffer,
			    m_Alloc);

			m_Buffer  = VK_NULL_HANDLE;
			m_Alloc   = VK_NULL_HANDLE;
			m_Address = 0;
			std::memset(&m_AllocInfo, 0, sizeof(VmaAllocationInfo));
		}
	}

	auto Buffer::allocation() -> VmaAllocation {
		return m_Alloc;
	}

	auto Buffer::allocation_info() -> VmaAllocationInfo {
		return m_AllocInfo;
	}

	Buffer& Buffer::operator=(Buffer&& other) {
		m_Buffer    = other.m_Buffer;
		m_Address   = other.m_Address;
		m_Alloc     = other.m_Alloc;
		m_AllocInfo = other.m_AllocInfo;

		other.m_Buffer    = VK_NULL_HANDLE;
		other.m_Address   = 0;
		other.m_Alloc     = VK_NULL_HANDLE;
		other.m_AllocInfo = {};

		return *this;
	}

	Buffer::operator VkBuffer() {
		return m_Buffer;
	}

	Buffer::operator vk::Buffer() {
		return m_Buffer;
	}

	Buffer::operator vk::DeviceAddress() {
		return m_Address;
	}

	VertexBuffer::VertexBuffer(size_t size, size_t stride) :
	    rhi::VertexBuffer(size, stride),
	    m_GPUData(size * stride,
	              vk::BufferUsageFlagBits::eVertexBuffer |
	                  vk::BufferUsageFlagBits::eTransferDst |
	                  vk::BufferUsageFlagBits::eShaderDeviceAddress,
	              VMA_MEMORY_USAGE_GPU_ONLY) {
	}

	auto VertexBuffer::upload() -> void {
		if (this->used_bytes() == 0) return;

		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
		vulkan::Buffer staging(
		    capacity_bytes(),
		    vk::BufferUsageFlagBits::eTransferSrc,
		    VMA_MEMORY_USAGE_CPU_ONLY);

		staging.write(m_Data, this->used_bytes());

		if (!staging.copy_to(m_GPUData, this->used_bytes())) {
			aby_rhi_err("failed to upload vertex buffer data");
		}
	}

	auto VertexBuffer::destroy() -> void {
		m_GPUData.destroy();
		if (m_Data) {
			std::free(m_Data);
			m_Data = nullptr;
		}
	}

	auto VertexBuffer::gpu() -> vulkan::Buffer& {
		return m_GPUData;
	}

	IndexBuffer::IndexBuffer(size_t size) :
	    rhi::IndexBuffer(size),
	    m_GPUData(size * sizeof(uint32_t),
	              vk::BufferUsageFlagBits::eIndexBuffer |
	                  vk::BufferUsageFlagBits::eTransferDst |
	                  vk::BufferUsageFlagBits::eShaderDeviceAddress,
	              VMA_MEMORY_USAGE_GPU_ONLY) {
	}

	auto IndexBuffer::upload() -> void {
		if (this->used_bytes() == 0) return;
		auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

		vulkan::Buffer staging(
		    capacity_bytes(),
		    vk::BufferUsageFlagBits::eTransferSrc,
		    VMA_MEMORY_USAGE_CPU_ONLY);

		void* data = staging.allocation_info().pMappedData;
		std::memcpy(data, m_Data, this->used_bytes());

		if (!r->immediate_submit([&](vk::CommandBuffer cmd) {
			vk::BufferCopy copy(0, 0, this->used_bytes());
			vkCmdCopyBuffer(
			    cmd,
			    staging,
			    m_GPUData,
			    1,
			    vkcast(copy));
		})) {
			aby_rhi_err("failed to upload vertex buffer data");
			return;
		}
	}

	auto IndexBuffer::destroy() -> void {
		m_GPUData.destroy();
		if (m_Data) {
			std::free(m_Data);
			m_Data = nullptr;
		}
	}

	auto IndexBuffer::gpu() -> vulkan::Buffer& {
		return m_GPUData;
	}

} // namespace aby::rhi::vulkan
