#pragma once
#include <vector>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

	auto create_surface(void* native_window, VkInstance instance, VkSurfaceKHR* surface) -> bool;
	auto get_instance_extensions(std::vector<const char*>* inst_exts) -> bool;
	auto get_device_extensions() -> std::vector<const char*>;
	auto get_extensions(std::vector<const char*>* inst_exts, std::vector<const char*>* dev_exts) -> bool;
} // namespace aby::rhi::vulkan
