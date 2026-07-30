#pragma once
#include "shader.hpp"
#include <vulkan/vulkan.hpp>
#include <unordered_map>

namespace aby::rhi::vulkan {


    class Shader : public aby::rhi::Shader {
    public:
        Shader(EShader type, const std::vector<uint32_t>& data);
        ~Shader();

        auto bind() -> void override;
        auto destroy() -> void override;
        auto module() -> vk::ShaderModule;
        auto type() const -> EShader override;

        auto set_uniform(std::string_view name, int32_t v) -> void override;
        auto set_uniform(std::string_view name, vec2<int32_t> v) -> void override;
        auto set_uniform(std::string_view name, vec3<int32_t> v) -> void override;
        auto set_uniform(std::string_view name, vec4<int32_t> v) -> void override;
        auto set_uniform(std::string_view name, float v) -> void override;
        auto set_uniform(std::string_view name, vec2<float> v) -> void override;
        auto set_uniform(std::string_view name, vec3<float> v) -> void override;
        auto set_uniform(std::string_view name, vec4<float> v) -> void override;

        auto add_input(const std::string& name, uint8_t location, size_t bytes) -> Shader& override;
        auto add_uniform(const std::string& name, size_t set, size_t binding, size_t bytes) -> Shader& override;
    protected:
        auto uniform_exists(std::string_view name) const -> bool;
    private:
        EShader               m_Type;
        vk::ShaderModule      m_Module;   
        std::vector<uint32_t> m_Data;
    };

}