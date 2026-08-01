#include "backends/vulkan/vulkan-shader.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"
#include "backends/vulkan/vulkan-common.hpp"
#include "context.hpp"

namespace aby::rhi::vulkan {

    Shader::Shader(EShader type, const std::vector<uint32_t>& data) :
        m_Type(type),
        m_Data(data)
    {
        auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());

        vk::ShaderModuleCreateInfo create_info(
            vk::ShaderModuleCreateFlags{},
            m_Data.size() * sizeof(uint32_t),    
            m_Data.data()
        );

        vkassert(vkCreateShaderModule(
            r->device(),
            vkcast(create_info),
            allocator(),
            vkcast(m_Module)
        ), "failed to create shader module: {}", type);
    }

    
    Shader::~Shader() {
        destroy();
    }

    auto Shader::destroy() -> void {
        if (m_Module) {
            auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
            vkDestroyShaderModule(r->device(), m_Module, allocator());
        }
    }


    auto Shader::bind() -> void {
    
    }
    
    auto Shader::type() const -> EShader {
        return m_Type;
    }

    auto Shader::module() -> vk::ShaderModule {
        return m_Module;
    }

}