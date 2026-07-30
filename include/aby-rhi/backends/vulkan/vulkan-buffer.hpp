#pragma once
#include "buffer.hpp"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>

namespace aby::rhi::vulkan {

    class Buffer {
    public:
        Buffer(size_t size, vk::BufferUsageFlags usage, VmaMemoryUsage memory_usage);
        ~Buffer();

        auto allocation() -> VmaAllocation;
        auto allocation_info() -> VmaAllocationInfo;
        auto destroy() -> void;

        operator VkBuffer();
        operator vk::Buffer();
        operator vk::DeviceAddress();
    private:
        vk::Buffer        m_Buffer;
        vk::DeviceAddress m_Address;
        VmaAllocation     m_Alloc;
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

}