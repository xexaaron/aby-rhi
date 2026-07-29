#pragma once
#include "context.hpp"
#include "common.hpp"
#include "backends/vulkan/vulkan-helpers.hpp"
#include "backends/vulkan/vulkan-callbacks.hpp"
#include "backends/vulkan/vulkan-platform.hpp"
#include <vulkan/vk_enum_string_helper.h>

#define __vkcheck__(result, return_expression, msg, ...)  do {                            \
        auto res = (result);                                                              \
        if (res != VK_SUCCESS) {                                                          \
            ::aby::rhi::Context::get().logger()->log(::aby::rhi::ELogLevel::error,        \
                std::format(msg __VA_OPT__(,) __VA_ARGS__));                              \
            ::aby::rhi::Context::get().logger()->log(::aby::rhi::ELogLevel::error,        \
                std::format("result: {}", string_VkResult(res)));                         \
            return_expression;                                                            \
        }                                                                                 \
    } while (0)        

#define vkcheck(result, msg, ... ) __vkcheck__(result, return false, msg __VA_OPT__(,) __VA_ARGS__)

#define vkassert(result, msg, ...) __vkcheck__(result, aby_rhi_assert(res == VK_SUCCESS, "vkassert failed"), msg __VA_OPT__(,) __VA_ARGS__)

#define vkbcheck(obj, msg) do {                                                   \
        if (!obj.has_value()) {                                                   \
            ::aby::rhi::vulkan::log_error(msg, obj.full_error());                 \
            return false;                                                         \
        }                                                                         \
    } while (0)

