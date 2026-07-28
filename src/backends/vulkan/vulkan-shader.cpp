#include "backends/vulkan/vulkan-shader.hpp"
#include "backends/vulkan/vulkan-renderer.hpp"
#include "backends/vulkan/vulkan-callbacks.hpp"
#include "backends/vulkan/vulkan-helpers.hpp"
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

        vkCreateShaderModule(
            r->device(),
            reinterpret_cast<VkShaderModuleCreateInfo*>(&create_info),
            allocator(),
            reinterpret_cast<VkShaderModule*>(&m_Module)
        );
    }

    
    Shader::~Shader() {
        auto* r = static_cast<vulkan::Renderer*>(Context::get().renderer());
        vkDestroyShaderModule(r->device(), m_Module, allocator());
    }

    auto Shader::bind() -> void {
    
    }
    
    auto Shader::type() const -> EShader {
        return m_Type;
    }

    auto Shader::module() -> vk::ShaderModule {
        return m_Module;
    }


    auto Shader::set_uniform(std::string_view name, int32_t v) -> void {
    }

    auto Shader::set_uniform(std::string_view name, vec2<int32_t> v) -> void {
    }
    
    auto Shader::set_uniform(std::string_view name, vec3<int32_t> v) -> void {
    }
    
    auto Shader::set_uniform(std::string_view name, vec4<int32_t> v) -> void {
    }
    
    auto Shader::set_uniform(std::string_view name, float v) -> void {
    }
    
    auto Shader::set_uniform(std::string_view name, vec2<float> v) -> void {
    }
    
    auto Shader::set_uniform(std::string_view name, vec3<float> v) -> void {
    }
    
    auto Shader::set_uniform(std::string_view name, vec4<float> v) -> void {
    }


    auto Shader::add_input(const std::string& name, uint8_t location, size_t bytes) -> Shader& {
        return *this;
    }

    auto Shader::add_uniform(const std::string& name, size_t set, size_t binding, size_t bytes) -> Shader& {
        return *this;
    }


}