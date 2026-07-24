#pragma once
#include "renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <VkBootstrap.h>

namespace aby::rhi::vulkan {

    class Renderer : public aby::rhi::IRenderer {
    public:
        auto init(void* native_window) -> bool override;
        auto deinit() -> void override;
        auto on_begin() -> bool override;
        auto on_end() -> void override;
        auto on_resize(uint32_t x, uint32_t y) -> void override;
    protected:
        auto get_extensions(std::vector<const char*>* inst_exts, std::vector<const char*>* dev_exts) -> bool;
    private:
        vkb::Device           m_Device;         
        vk::SurfaceKHR        m_Surface;
        vkb::Instance         m_Instance;
        vk::Queue             m_GraphicsQueue;
        uint32_t              m_GraphicsQueueFamily;
        vk::Queue             m_PresentQueue;
        uint32_t              m_PresentQueueFamily;
        vkb::Swapchain        m_Swapchain;
    };

}