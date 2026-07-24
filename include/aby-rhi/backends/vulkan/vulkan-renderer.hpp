#pragma once
#include "renderer.hpp"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include <VkBootstrap.h>
#include <array>

namespace aby::rhi::vulkan {

    struct SwapchainImage {
        vk::Image     img;
        vk::ImageView view;
        vk::Semaphore render_finished;
    };

    struct AllocatedImage {
        vk::Image     img;
        vk::ImageView view;
        VmaAllocation alloc;
        vk::Extent3D  extent;
        vk::Format    format;
    };

    struct FrameData {
        vk::CommandPool   pool;
        vk::CommandBuffer cmd;
        vk::Semaphore     acquire;
        vk::Fence         render_fence;
    };

    static constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;

    class Renderer : public aby::rhi::IRenderer {
    public:
        auto init(void* native_window) -> bool override;
        auto deinit() -> void override;
        auto set_clear_color(Color color) -> void override;
        auto on_begin() -> bool override;
        auto on_end() -> bool override;
        auto on_resize(uint32_t width, uint32_t height) -> void override;
    protected:
        auto recreate_swapchain() -> bool;
        auto get_extensions(std::vector<const char*>* inst_exts, std::vector<const char*>* dev_exts) -> bool;
        auto get_current_frame() -> FrameData&;
    private:
        uint32_t                    m_GraphicsQueueFamily   = UINT32_MAX;
        uint32_t                    m_PresentQueueFamily    = UINT32_MAX;
        uint32_t                    m_FrameIndex            = 0;                    
        uint32_t                    m_SwapchainImgIndex     = UINT32_MAX;
        uint32_t                    m_Width                 = 800;
        uint32_t                    m_Height                = 600;
        VmaAllocator                m_VMA                   = VK_NULL_HANDLE;
        vk::SurfaceKHR              m_Surface;
        vk::Queue                   m_GraphicsQueue;
        vk::Queue                   m_PresentQueue;
        vkb::Swapchain              m_Swapchain;
        vkb::Device                 m_Device;         
        vkb::Instance               m_Instance;
        vk::ClearColorValue         m_ClearColor;
        AllocatedImage              m_DrawImage;
        std::vector<SwapchainImage> m_Images;
        std::array<FrameData, MAX_FRAMES_IN_FLIGHT> m_Frames;
    };

}