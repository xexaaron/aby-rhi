#pragma once
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

    auto load_functions(vk::Instance instance, vk::Device device) -> void;

}