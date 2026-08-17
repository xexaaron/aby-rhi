

# File vulkan-renderer.hpp

[**File List**](files.md) **>** [**aby-rhi**](dir_5ec795ce3784ed21409c94a662c670e0.md) **>** [**backends**](dir_b2e089009e3d6019946b366985df1d18.md) **>** [**vulkan**](dir_8ecac3f6860031a9c88024a6c0dd2404.md) **>** [**vulkan-renderer.hpp**](vulkan-renderer_8hpp.md)

[Go to the documentation of this file](vulkan-renderer_8hpp.md)


```C++
#pragma once
#include "backends/vulkan/vulkan-descriptors.hpp"
#include "backends/vulkan/vulkan-frame.hpp"
#include "backends/vulkan/vulkan-gc.hpp"
#include "backends/vulkan/vulkan-image.hpp"
#include "backends/vulkan/vulkan-immediate-cmd.hpp"
#include "backends/vulkan/vulkan-pipeline.hpp"
#include "backends/vulkan/vulkan-render-pass.hpp"
#include "backends/vulkan/vulkan-shader.hpp"
#include "renderer.hpp"

#include <VkBootstrap.h>
#include <array>
#include <functional>
#include <vk_mem_alloc.h>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

    class Renderer : public aby::rhi::Renderer {
    public:
        Renderer(GraphicsParams params);
        ~Renderer() = default;

        auto init(void* native_window) -> bool override;
        auto deinit() -> void override;
        auto set_clear_color(Color color) -> void override;
        auto add_pass(std::shared_ptr<rhi::RenderPass> render_pass) -> void override;

        auto immediate_submit(std::function<void(vk::CommandBuffer)>&& fn) -> bool;
        auto register_texture(ResourceID id, vk::ImageView view, vk::Sampler smapler) -> uint32_t;
        auto update_texture(uint32_t texture_id, vk::ImageView view, vk::Sampler sampler) -> void;

        auto on_begin() -> bool override;
        auto on_end() -> bool override;
    public:
        auto device() -> vkb::Device&;
        auto vma() -> VmaAllocator&;
        auto color_format() -> vk::Format;
        auto gc() -> GarbageCollector&;
        auto desc_alloc() -> DescriptorAllocator&;
        auto tex_desc_set() -> vk::DescriptorSet;
        auto tex_desc_layout() -> vk::DescriptorSetLayout;
        auto max_sampler_anisotropy() -> float;
        auto graphics() const -> const GraphicsParams&;
        auto width() const -> uint32_t;
        auto height() const -> uint32_t;
        auto clear_color() const -> vk::ClearColorValue;
        auto frame_index() const -> size_t;
        auto get_resolve_attachment(rhi::Texture* color_attachment) -> rhi::Texture*;
    private:
        auto init_vulkan(void* native_window) -> bool;
        auto init_vma() -> bool;
        auto init_descriptors() -> bool;
    protected:
        auto recreate_swapchain() -> bool;
        auto get_immediate() -> ImmediateCommands&;
    private:
        GraphicsParams m_Graphics;
        void* m_Window;

        uint32_t m_GraphicsQueueFamily = UINT32_MAX;
        uint32_t m_PresentQueueFamily  = UINT32_MAX;
        uint32_t m_FrameIndex          = 0;
        uint32_t m_SwapchainImgIndex   = UINT32_MAX;
        uint32_t m_Width               = 800;
        uint32_t m_Height              = 600;
        VmaAllocator m_VMA             = VK_NULL_HANDLE;
        DescriptorAllocator m_DescAllocator;
        DescriptorAllocator m_TexAllocator;

        vk::SurfaceKHR m_Surface         = VK_NULL_HANDLE;
        vk::Queue m_GraphicsQueue        = VK_NULL_HANDLE;
        vk::Queue m_PresentQueue         = VK_NULL_HANDLE;
        vkb::Swapchain m_Swapchain       = {};
        vkb::Device m_Device             = {};
        vkb::Instance m_Instance         = {};
        vk::ClearColorValue m_ClearColor = vk::ClearColorValue(0.15f, 0.15f, 0.15f, 1.f);

        vk::DescriptorSet m_DrawImageDescriptors            = VK_NULL_HANDLE;
        vk::DescriptorSetLayout m_DrawImageDescriptorLayout = VK_NULL_HANDLE;
        vk::DescriptorSet m_TextureDescriptors              = VK_NULL_HANDLE;
        vk::DescriptorSetLayout m_TextureDescriptorLayout   = VK_NULL_HANDLE;

        std::mutex m_ImmediateSubmitMutex;

        vk::PhysicalDeviceLimits m_Limits;

        GarbageCollector m_GC;

        std::vector<std::pair<Image, vk::Semaphore>> m_SwapchainImages;
        Frames m_Frames;

        std::vector<std::shared_ptr<RenderPass>> m_RenderPasses;
        std::unordered_map<rhi::Texture*, rhi::Texture*> m_ColorToResolveAttachment;

        RenderPass* m_PresentPass = nullptr;
    };

} // namespace aby::rhi::vulkan
```


