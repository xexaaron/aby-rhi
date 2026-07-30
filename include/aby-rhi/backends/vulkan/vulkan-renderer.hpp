#pragma once
#include "renderer.hpp"
#include "backends/vulkan/vulkan-descriptors.hpp"
#include "backends/vulkan/vulkan-shader.hpp"
#include "backends/vulkan/vulkan-pipeline.hpp"
#include "backends/vulkan/vulkan-render-pass.hpp"
#include "backends/vulkan/vulkan-gc.hpp"
#include <vulkan/vulkan.hpp>
#include <vk_mem_alloc.h>
#include <VkBootstrap.h>
#include <array>
#include <functional>

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

    struct ImmediateCommands {
        vk::Fence         fence;
        vk::CommandBuffer cmd;
        vk::CommandPool   pool;
    };

    static constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;

    class Renderer : public aby::rhi::IRenderer {
    public:
        Renderer() { }
        ~Renderer() = default;

        auto init(void* native_window) -> bool override;
        auto deinit() -> void override;
        auto set_clear_color(Color color) -> void override;
        auto add_pass(std::shared_ptr<rhi::RenderPass> render_pass) -> void override;
        
        auto immediate_submit(std::function<void(vk::CommandBuffer)>&& fn) -> bool;
         
        auto on_begin() -> bool override;
        auto on_end() -> bool override;
        auto on_resize(uint32_t width, uint32_t height) -> void override;
    public:
        auto device() -> vkb::Device&;
        auto vma() -> VmaAllocator&;
        auto color_format() -> vk::Format;
        auto gc() -> GarbageCollector&;
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
        DescriptorAllocator         m_DescAllocator;
        vk::SurfaceKHR              m_Surface               = VK_NULL_HANDLE;
        vk::Queue                   m_GraphicsQueue         = VK_NULL_HANDLE;
        vk::Queue                   m_PresentQueue          = VK_NULL_HANDLE;
        vkb::Swapchain              m_Swapchain             = {};
        vkb::Device                 m_Device                = {};         
        vkb::Instance               m_Instance              = {};
        vk::ClearColorValue         m_ClearColor            = {};
        ImmediateCommands           m_Immediate             = {};

        AllocatedImage              m_DrawImage                 = {};
        vk::DescriptorSet           m_DrawImageDescriptors      = VK_NULL_HANDLE;
        vk::DescriptorSetLayout     m_DrawImageDescriptorLayout = VK_NULL_HANDLE;
 
        GarbageCollector            m_GC;

        std::vector<SwapchainImage> m_Images;
        std::array<FrameData, MAX_FRAMES_IN_FLIGHT> m_Frames;

        std::vector<std::shared_ptr<RenderPass>> m_RenderPasses;
    };

}