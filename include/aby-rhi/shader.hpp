#pragma once
#include "interfaces/interfaces.hpp"

namespace aby::rhi {

    class Shader {
    public:
        /**
         * @brief The type of shader will be determined by the path.
         *        pre-compiled shaders should be suffixed by '.spv' (ex. .vert.spv)
         * @param .geom Geometry shader
         * @param .vert Vertex shader
         * @param .frag Fragment shader
         * @param .comp Compute shader
        */
        static auto create(const fs::path& rel_path) -> std::shared_ptr<Shader>;
        virtual ~Shader() = default;
        /**
         * @brief Bind the shader. 
         * @warning Must be called during the frame, not outside of it. 
        */
        virtual auto bind() -> void = 0;
    protected:
        Shader(const fs::path& rel_path); 
    private:
    };

};