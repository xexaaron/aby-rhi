#include "backends/vulkan/vulkan-buffer.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"
#include "context.hpp"

#include <vk_mem_alloc.h>

namespace aby::rhi::vulkan {

    Buffer::Buffer(size_t size, vk::BufferUsageFlags usage, VmaMemoryUsage memory_usage) {
        vk::BufferCreateInfo create_info(
            vk::BufferCreateFlags(),
            size,
            usage
        );

        VmaAllocationCreateInfo alloc_create_info = {};
        alloc_create_info.usage = memory_usage;
        alloc_create_info.flags = VMA_ALLOCATION_CREATE_MAPPED_BIT;

        auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer()); 
        
        vmaCreateBuffer(
            r->vma(),
            reinterpret_cast<VkBufferCreateInfo*>(&create_info),
            &alloc_create_info,
            reinterpret_cast<VkBuffer*>(&m_Buffer),
            &m_Alloc,
            &m_AllocInfo
        );

        vk::BufferDeviceAddressInfo info(m_Buffer);
        m_Address = vkGetBufferDeviceAddress(
            r->device(),
            reinterpret_cast<VkBufferDeviceAddressInfo*>(&info)
        );
    }

    Buffer::~Buffer() {
        auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer()); 
        vmaDestroyBuffer(
            r->vma(), 
            m_Buffer,
            m_Alloc
        );
    }

    auto Buffer::allocation() -> VmaAllocation {
        return m_Alloc;
    }

    auto Buffer::allocation_info() -> VmaAllocationInfo {
        return m_AllocInfo;
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
        m_GPUData(size, 
            vk::BufferUsageFlagBits::eVertexBuffer |
            vk::BufferUsageFlagBits::eTransferDst,         
            VMA_MEMORY_USAGE_GPU_ONLY
        )
    {

    }

    auto VertexBuffer::upload() -> void  {
        auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer()); 

        vulkan::Buffer staging(
            capacity_bytes(),
            vk::BufferUsageFlagBits::eTransferSrc,
            VMA_MEMORY_USAGE_CPU_ONLY
        );

        void* data = staging.allocation_info().pMappedData;
        std::memcpy(data, m_Data.data(), this->used_bytes());

        if (!r->immediate_submit([&](vk::CommandBuffer cmd){
            vk::BufferCopy copy(0, 0, this->used_bytes());
            cmd.copyBuffer(staging, m_GPUData, { copy });
        })) {
            aby_rhi_err("failed to upload vertex buffer data");
            return;
        }
    }

    IndexBuffer::IndexBuffer(size_t size) :
        rhi::IndexBuffer(size),
        m_GPUData(size, 
            vk::BufferUsageFlagBits::eIndexBuffer |
            vk::BufferUsageFlagBits::eTransferDst,
            VMA_MEMORY_USAGE_GPU_ONLY
        )
    {

    }    
    
    auto IndexBuffer::upload() -> void  {
        auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer()); 

        vulkan::Buffer staging(
            capacity_bytes(),
            vk::BufferUsageFlagBits::eTransferSrc,
            VMA_MEMORY_USAGE_CPU_ONLY
        );

        void* data = staging.allocation_info().pMappedData;
        std::memcpy(data, m_Data.data(), this->used_bytes());

        if (!r->immediate_submit([&](vk::CommandBuffer cmd){
            vk::BufferCopy copy(0, 0, this->used_bytes());
            cmd.copyBuffer(staging, m_GPUData, { copy });
        })) {
            aby_rhi_err("failed to upload vertex buffer data");
            return;
        }
    }




}