#pragma once
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

	/**
	* @brief Use the vulkan dynamic dispatcher to load function pointers
	* @param[in] instance the vulkan instance
	* @param[in] device the vulkan logical device
	*/
	auto load_functions(vk::Instance instance, vk::Device device) -> void;

} // namespace aby::rhi::vulkan
