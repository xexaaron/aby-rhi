#pragma once
#include "backends/vulkan/vulkan-callbacks.hpp"
#include "backends/vulkan/vulkan-helpers.hpp"
#include "backends/vulkan/vulkan-platform.hpp"
#include "common.hpp"
#include "context.hpp"

#include <vulkan/vk_enum_string_helper.h>

/**
 * @brief private vulkan check macro that handles different return expressions
 * @param result VkResult from a function or variable
 * @param return_expression What happens when the check fails
 * @param msg formmatable message to log 
 * @param ... format msg arguments
 */
#define __vkcheck__(result, return_expression, msg, ...)                                               \
	do {                                                                                               \
		auto res = (result);                                                                           \
		if (res != VK_SUCCESS) {                                                                       \
			::aby::rhi::Context::get().logger()->log(::aby::rhi::ELogLevel::error,                     \
			                                         std::format(msg __VA_OPT__(, ) __VA_ARGS__));     \
			::aby::rhi::Context::get().logger()->log(::aby::rhi::ELogLevel::error,                     \
			                                         std::format("result: {}", string_VkResult(res))); \
			return_expression;                                                                         \
		}                                                                                              \
	} while (0)

/**
 * @brief vulkan result check macro that handles different return expressions
 * @param result VkResult from a function or variable
 * @param msg formmatable message to log 
 * @param ... format msg arguments
 * @return false on failure. otherwise nothing.
 */
#define vkcheck(result, msg, ...) __vkcheck__(result, return false, msg __VA_OPT__(, ) __VA_ARGS__)

/**
 * @brief vulkan result check macro that handles different return expressions
 * @param result VkResult from a function or variable
 * @param msg formmatable message to log 
 * @param ... format msg arguments
 * @return assertion on failure. otherwise nothing.
 */
#define vkassert(result, msg, ...) __vkcheck__(result, aby_rhi_assert(res == VK_SUCCESS, "vkassert failed"), msg __VA_OPT__(, ) __VA_ARGS__)

/**
 * @brief vulkan result check macro that propogates the error to the caller
 * @param result VkResult from a function or variable
 * @param msg formattable message to log
 * @param ... format msg arguments
 * @return error that was logged, otherwise nothing
 */
#define vkpropagate(result, msg, ...) __vkcheck__(result, return static_cast<vk::Result>(res), msg __VA_OPT__(, ) __VA_ARGS__)

/**
 * @brief vulkan bootstrap check macro 
 * @param obj The Result<T> object from a vkb:: Builder
 * @param msg The message to log as the header for the error
 * @return false on failure. otherwise nothing
 */
#define vkbcheck(obj, msg)                                        \
	do {                                                          \
		if (!obj.has_value()) {                                   \
			::aby::rhi::vulkan::log_error(msg, obj.full_error()); \
			return false;                                         \
		}                                                         \
	} while (0)

namespace aby::rhi::vulkan {

	/**
     * @brief Helper function for converting vk:: namespaced types from vulkan.hpp to
     *        their Vulkan C counterparts
     * @param obj reference to a vk:: namespaced object
     * @return Vulkan-C pointee 
     */
	template <typename T>
	auto vkcast(T& obj) -> typename std::remove_cvref_t<T>::NativeType* {
		using U = std::remove_cvref_t<T>;
		return reinterpret_cast<typename U::NativeType*>(&obj);
	}

	/**
     * @brief Helper function for converting vk:: namespaced types from vulkan.hpp to
     *        their Vulkan C counterparts
     * @param obj pointer to a vk:: namespaced object
     * @return Vulkan-C pointee 
     */
	template <typename T>
	auto vkcast(T* obj) -> typename std::remove_cvref_t<T>::NativeType* {
		using U = std::remove_cvref_t<T>;
		return reinterpret_cast<typename U::NativeType*>(obj);
	}

	template <typename T>
	auto vkconvert(T value) {
		if constexpr (std::same_as<T, EFormat>) {
			return eformat_to_vkformat(value);
		} else if constexpr (std::same_as<T, ERepeatMode>) {
			return erepeatmode_to_vkrepeatmode(value);
		} else if constexpr (std::same_as<T, EFiltering>) {
			return efiltering_to_vkfilter(value);
		} else if constexpr (std::same_as<T, EBlendOp>) {
			return eblendop_to_vkblendop(value);
		} else if constexpr (std::same_as<T, EBlendFactor>) {
			return eblendfactor_to_vkblendfactor(value);
		} else if constexpr (std::same_as<T, ECompareOp>) {
			return ecompareop_to_vkcompareop(value);
		} else if constexpr (std::same_as<T, ECullMode>) {
			return ecullmode_to_vkcullmode(value);
		} else if constexpr (std::same_as<T, EFrontFace>) {
			return efrontface_to_vkfrontface(value);
		} else if constexpr (std::same_as<T, ETopology>) {
			return etopology_to_vktopology(value);
		} else if constexpr (std::same_as<T, EPolygonMode>) {
			return epolygonmode_to_vkpolygonmode(value);
		} else if constexpr (std::same_as<T, EShader>) {
			return eshader_to_vkshader(value);
		} else {
			static_assert(std::false_type::value, "No Vulkan conversion available for type");
		}
	}

} // namespace aby::rhi::vulkan
