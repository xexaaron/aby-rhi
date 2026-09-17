#pragma once
#include "common.hpp"

#include <vector>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

	/**
	* @brief Create the surface from the native window
	* @param[in] native_window the type is determined from the @c Context
	* @param[in] instance the vulkan instance
	* @param[out] surface the created vulkan surface, or nullptr on failure
	* @return true on success, otherwise false
	*/
	auto ABY_RHI_API create_surface(void* native_window, VkInstance instance, VkSurfaceKHR* surface) -> bool;
	/**
	* @brief Get the vulkan instance extensions required by the backend
	* @param[out] inst_exts the instance extensions, or empty on failure
	* @return true on success, otherwise false
	*/
	auto ABY_RHI_API get_instance_extensions(std::vector<const char*>* inst_exts) -> bool;
	/**
	* @brief Get the vulkan device extensions required by the backend
	* @return vector<const char*>
	*/
	auto ABY_RHI_API get_device_extensions() -> std::vector<const char*>;
	/**
	* @brief Get the vulkan instance and device extensions required by the backend
	* @param[out] inst_exts the instance extensions, or empty on failure
	* @param[out] dev_exts the device extensions
	* @return true on success, otherwise false
	*/
	auto ABY_RHI_API get_extensions(std::vector<const char*>* inst_exts, std::vector<const char*>* dev_exts) -> bool;
	/**
	* @brief Get the window size natively for swapchain resizing
	* @param[in] native_window the type will be determined from the @c Context
	* @param[out] w the window width, or 0 on failure
	* @param[out] h the window height, or 0 on failure
	* @note This does not work on wayland as querying window size is not allowed
	*/
	auto ABY_RHI_API get_window_size(void* native_window, uint32_t* w, uint32_t* h) -> void;

#ifdef __linux__
	/**
	* @brief Set the wayland window get size callback
	* @note Because we cannot track the wayland window size ourselves this is required.
	* @param cb a function ptr that gets the window size
	*/
	auto ABY_RHI_API set_wayland_get_size_callback(void (*cb)(uint32_t* w, uint32_t* h)) -> void;
#endif

} // namespace aby::rhi::vulkan
