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

	class Shader {
	public:
		/**
         * @brief The type of shader will be determined by the path.
         * @warning Do not use precompiled shaders. For vertex shaders we want to be able to parse a description.
         * @param .geom Geometry shader
         * @param .vert Vertex shader
         * @param .frag Fragment shader
         * @param .comp Compute shader
        */
		static auto create(const fs::path& rel_path) -> Resource;
		virtual ~Shader() = default;

		virtual auto data() -> std::span<uint32_t> = 0;
		virtual auto bind() -> void                = 0;
		virtual auto destroy() -> void             = 0;
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
