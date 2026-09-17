#pragma once
#include "interfaces/interfaces.hpp"
#include "resource.hpp"

#include <array>

namespace aby::rhi {

	struct VertexAttributeDesc {
		uint8_t location;
		size_t bytes;
		std::string name;
	};

	struct UniformBufferDesc {
		size_t set;
		size_t binding;
		size_t bytes;
		std::string name;
	};

	struct VertexShaderDesc {
		std::vector<VertexAttributeDesc> inputs;
		std::vector<UniformBufferDesc> uniforms;
	};

	class ABY_RHI_API Shader {
	public:
		/**
         * @brief Create a shader from a file.
		 * @note The type of shader will be determined by the path.
         * @warning Do not use precompiled shaders.
         * @param rel_path path relative to the cwd
		 * @param .geom Geometry shader
         * @param .vert Vertex shader
         * @param .frag Fragment shader
         * @param .comp Compute shader
        */
		static auto create(const fs::path& rel_path) -> ResourcePtr<Shader, EResource::shader>;
		/**
		* @brief Create a shader from source code
		* @warning Do not use precompiled shaders
		* @param name the name of the shader (used for caching)
		* @param source_code string of valid glsl source code
		* @param type the shader stage
		*/
		static auto create(const std::string& name, const std::string& source_code, EShader type) -> ResourcePtr<Shader, EResource::shader>;
		virtual ~Shader()                          = default;
		/**
		* @brief Get the shader code
		*/
		virtual auto data() -> std::span<uint32_t> = 0;
		/**
		* @brief Bind the shader, used in backends like opengl
		*/
		virtual auto bind() -> void                = 0;
		/**
		* @brief Destroy the shader and any backend data
		*/
		virtual auto destroy() -> void             = 0;
		/**
		* @brief Get the shader stage
		*/
		virtual auto type() const -> EShader       = 0;
	protected:
		struct PathData {
			std::string name;
			fs::path ext;
			bool is_compiled;
		};
	protected:
		static auto ext_to_eshader(const fs::path& ext) -> EShader;
		static auto get_path_data(const fs::path& rel_path) -> PathData;
		static auto is_cached_shader(fs::path rel_path) -> bool;
		static auto size_of_glsl_type(const std::string& glsl_type) -> size_t;
	};

	using ShaderPtr = ResourcePtr<Shader, EResource::shader>;

} // namespace aby::rhi
