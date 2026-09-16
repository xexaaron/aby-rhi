#pragma once
#include "common.hpp"

#include <vector>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

	auto ABY_RHI_API create_surface(void* native_window, VkInstance instance, VkSurfaceKHR* surface) -> bool;
	auto ABY_RHI_API get_instance_extensions(std::vector<const char*>* inst_exts) -> bool;
	auto ABY_RHI_API get_device_extensions() -> std::vector<const char*>;
	auto ABY_RHI_API get_extensions(std::vector<const char*>* inst_exts, std::vector<const char*>* dev_exts) -> bool;
	auto ABY_RHI_API get_window_size(void* native_window, uint32_t* x, uint32_t* y) -> void;

#ifdef __linux__
	auto ABY_RHI_API set_wayland_get_size_callback(void (*cb)(uint32_t* w, uint32_t* h)) -> void;
#endif

} // namespace aby::rhi::vulkan
