#pragma once
#include "shader.hpp"

#include <unordered_map>
#include <vulkan/vulkan.hpp>

namespace aby::rhi::vulkan {

	class Shader : public aby::rhi::Shader {
	public:
		Shader(EShader type, const std::vector<uint32_t>& data);
		~Shader();

		auto data() -> std::span<uint32_t>;
		auto bind() -> void override;
		auto destroy() -> void override;
		auto module() -> vk::ShaderModule;
		auto type() const -> EShader override;
	private:
		EShader m_Type;
		vk::ShaderModule m_Module;
		std::vector<uint32_t> m_Data;
	};

} // namespace aby::rhi::vulkan
