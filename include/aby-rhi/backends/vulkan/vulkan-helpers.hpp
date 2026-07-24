#pragma once
#include <VkBootstrap.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vk_enum_string_helper.h>

#define vkcheck(result, msg) do {                                                         \
        auto res = (result);                                                              \
        if (res != VK_SUCCESS) {                                                          \
            ::aby::rhi::Context::get().logger()->log(::aby::rhi::ELogLevel::error, msg);  \
            ::aby::rhi::Context::get().logger()->log(::aby::rhi::ELogLevel::error,        \
                std::format("result: {}", string_VkResult(res)));                         \
            return false;                                                                 \
        }                                                                                 \
    } while (0)                                                                    

#define vkbcheck(obj, msg) do {                                                   \
        if (!obj.has_value()) {                                                   \
            ::aby::rhi::vulkan::log_error(msg, obj.full_error());                 \
            return false;                                                         \
        }                                                                         \
    } while (0)       

namespace aby::rhi::vulkan {

    auto transition_image(vk::CommandBuffer cmd, vk::Image image, vk::ImageLayout src, vk::ImageLayout dst) -> void;
    auto copy_image_to_image(vk::CommandBuffer cmd, vk::Image src, vk::Extent2D src_sz, vk::Image dst, vk::Extent2D dst_sz) -> void;
    auto clear_screen(vk::CommandBuffer cmd, vk::Image image, vk::ImageLayout layout, vk::ClearColorValue& color, vk::ImageSubresourceRange& range) -> void;
    auto log_error(const std::string& message, const vkb::Error& error) -> void;
}