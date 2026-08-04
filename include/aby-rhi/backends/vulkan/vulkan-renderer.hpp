#pragma once
#include "backends/vulkan/vulkan-descriptors.hpp"
#include "backends/vulkan/vulkan-gc.hpp"
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

	struct SwapchainImage {
		vk::Image img                 = VK_NULL_HANDLE;
		vk::ImageView view            = VK_NULL_HANDLE;
		vk::Semaphore render_finished = VK_NULL_HANDLE;
	};

	struct AllocatedImage {
		vk::Image img       = VK_NULL_HANDLE;
		vk::ImageView view  = VK_NULL_HANDLE;
		VmaAllocation alloc = VK_NULL_HANDLE;
		vk::Extent3D extent = {};
		vk::Format format   = vk::Format::eUndefined;
	};

	struct FrameData {
		vk::CommandPool pool   = VK_NULL_HANDLE;
		vk::CommandBuffer cmd  = VK_NULL_HANDLE;
		vk::Semaphore acquire  = VK_NULL_HANDLE;
		vk::Fence render_fence = VK_NULL_HANDLE;
	};

	struct ImmediateCommands {
		~ImmediateCommands();

		auto destroy() -> void;

		vk::Fence fence       = VK_NULL_HANDLE;
		vk::CommandBuffer cmd = VK_NULL_HANDLE;
		vk::CommandPool pool  = VK_NULL_HANDLE;
	};

	static constexpr size_t MAX_FRAMES_IN_FLIGHT = 2;

	class Renderer : public aby::rhi::Renderer {
	public:
		Renderer() {
		}
		~Renderer() = default;

		auto init(void* native_window) -> bool override;
		auto deinit() -> void override;
		auto set_clear_color(Color color) -> void override;
		auto add_pass(std::shared_ptr<rhi::RenderPass> render_pass) -> void override;

		auto immediate_submit(std::function<void(vk::CommandBuffer)>&& fn) -> bool;
		auto register_texture(vk::ImageView view, vk::Sampler smapler) -> uint32_t;

		auto on_begin() -> bool override;
		auto on_end() -> bool override;
		auto on_resize(uint32_t width, uint32_t height) -> void override;
	public:
		auto device() -> vkb::Device&;
		auto vma() -> VmaAllocator&;
		auto color_format() -> vk::Format;
		auto gc() -> GarbageCollector&;
		auto desc_alloc() -> DescriptorAllocator&;
		auto tex_desc_set() -> vk::DescriptorSet;
		auto tex_desc_layout() -> vk::DescriptorSetLayout;
	private:
		auto init_vulkan(void* native_window) -> bool;
		auto init_commands() -> bool;
		auto init_vma() -> bool;
		auto init_draw_image() -> bool;
		auto init_descriptors() -> bool;
	protected:
		auto recreate_swapchain() -> bool;
		auto get_extensions(std::vector<const char*>* inst_exts, std::vector<const char*>* dev_exts) -> bool;
		auto get_current_frame() -> FrameData&;
		auto get_immediate() -> ImmediateCommands&;
		auto create_immediate_commands() -> ImmediateCommands;
	private:
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

		AllocatedImage m_DrawImage                          = {};
		vk::DescriptorSet m_DrawImageDescriptors            = VK_NULL_HANDLE;
		vk::DescriptorSetLayout m_DrawImageDescriptorLayout = VK_NULL_HANDLE;

		vk::DescriptorSet m_TextureDescriptors            = VK_NULL_HANDLE;
		vk::DescriptorSetLayout m_TextureDescriptorLayout = VK_NULL_HANDLE;

		std::mutex m_ImmediateSubmitMutex;

		GarbageCollector m_GC;

		std::vector<SwapchainImage> m_Images;
		std::array<FrameData, MAX_FRAMES_IN_FLIGHT> m_Frames;

		std::vector<std::shared_ptr<RenderPass>> m_RenderPasses;
	};

} // namespace aby::rhi::vulkan
