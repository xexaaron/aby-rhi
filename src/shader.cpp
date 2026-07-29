#include "shader.hpp"
#include <iostream>

#include "context.hpp"
#include "backends/vulkan/vulkan-shader.hpp"
#include <shaderc/shaderc.hpp>
#include <assert.h>

namespace aby::rhi {

    auto eshader_to_shaderc(EShader type) -> shaderc_shader_kind;

    auto Shader::create(const fs::path& rel_path) -> std::shared_ptr<Shader> {
        // Officially we do not support using precompiled shaders here.
        // We want to cache them ourselves.
        // For vertex shaders we want to build a descriptor layout from them.


        auto& ctx = Context::get();
        auto* io  = ctx.file_io();
        auto* log = ctx.logger();

        if (!fs::exists(io->cwd() / rel_path)) {
            aby_rhi_err("file does not exist: {}", (io->cwd() / rel_path).string());
            return nullptr;
        }

        auto [name, ext, is_compiled] = get_path_data(rel_path);
        
        EShader type = ext_to_eshader(ext);
        if (type == EShader::none) {
            aby_rhi_err("unsupported shader extension type: {}", ext.string());
            aby_rhi_err("expected one of [.vert, .frag, .comp, .geom]");
        } 

        bool is_cached = is_cached_shader(io->cache_dir() / rel_path);

        fs::path read_path = rel_path;

        if (!is_compiled) {
            if (!is_cached) {
                if (!compile_shader(rel_path, name, type)) {
                    aby_rhi_err("failed to compile shader: {}", name);
                    return nullptr;
                }
            }  

            read_path = io->cache_dir() / rel_path.string().append(".spv"); 
        }

        std::vector<uint32_t> data; 
        if (!io->read(read_path, &data)) {
            aby_rhi_err("failed to read shader file: {}", read_path.string());
            return nullptr;
        }

        switch (ctx.renderer_backend()) {
            case ERenderer::vulkan: {
                return std::make_shared<vulkan::Shader>(type, data);
            }
            default:
                aby_rhi_ftl("renderer backend shader not implemented!");
                assert(false && "renderer backend shader not implemented");
        }

        aby_rhi_err("failed to create shader: {}", rel_path.string());
        return nullptr;
    } 

    auto Shader::compile_shader(const fs::path& rel_path, const std::string& name, EShader type) -> bool {
        auto& ctx = Context::get();
        auto* io  = ctx.file_io();
        auto* log = ctx.logger();

        std::vector<uint8_t> data;
        if (!io->read(rel_path, &data)) {
            return false;
        }
        
        auto shaderc_type = eshader_to_shaderc(type);
        std::string source(
            reinterpret_cast<char*>(data.data()),
            data.size()
        );


        shaderc::Compiler compiler;
        shaderc::CompileOptions options;
#ifdef NDEBUG
        options.SetOptimizationLevel(shaderc_optimization_level_performance);
#else
        options.SetOptimizationLevel(shaderc_optimization_level_size);
#endif
        // TODO: renderer agnostic
        options.SetTargetEnvironment(shaderc_target_env_vulkan, shaderc_env_version_vulkan_1_4);

        auto module = compiler.CompileGlslToSpv(
            source,
            eshader_to_shaderc(type), 
            name.c_str(),
            options
        );

        if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
            log->log(ELogLevel::error, module.GetErrorMessage());
            return false;
        }


        std::vector<uint32_t> out_data{module.cbegin(), module.cend()};
        fs::path out_path = io->cache_dir() / (rel_path.string().append(".spv"));

        if (!io->write(out_path, out_data)) {
            return false;
        }

#ifndef NDEBUG
        aby_rhi_dbg("compiled shader: {}", name);
#endif
        return true;
    }

    auto Shader::ext_to_eshader(const fs::path& ext) -> EShader {
        if      (ext == ".vert") return EShader::vert;
        else if (ext == ".frag") return EShader::frag;
        else if (ext == ".comp") return EShader::comp;
        else if (ext == ".geom") return EShader::geom;
        return EShader::none;
    }

    auto eshader_to_shaderc(EShader type) -> shaderc_shader_kind {
        switch (type) {
            case EShader::vert:
                return shaderc_vertex_shader; 
                break; 
            case EShader::frag: 
                return shaderc_fragment_shader;
                break;
            case EShader::comp: 
                return shaderc_compute_shader;
                break;
            case EShader::geom: 
                return shaderc_geometry_shader;
                break;
            default:
                return shaderc_glsl_infer_from_source;
        }
        return shaderc_glsl_infer_from_source;
    }

    auto Shader::get_path_data(const fs::path& rel_path) -> PathData {
        bool is_compiled = rel_path.extension() == ".spv";
        fs::path ext = is_compiled ? rel_path.stem().extension() : rel_path.extension();
        std::string name = is_compiled ? 
            rel_path.stem().filename().replace_extension("").string() :
            rel_path.filename().replace_extension("").string();

        return PathData{
            .name = name,
            .ext  = ext,
            .is_compiled = is_compiled
        };
    } 

    auto Shader::is_cached_shader(fs::path rel_path) -> bool {
        auto cache_dir = Context::get().file_io()->cache_dir();
        fs::path path = cache_dir / rel_path.string().append(".spv");
        return fs::exists(path);
    }

    auto Shader::size_of_glsl_type(const std::string& glsl_type) -> size_t {
        if (glsl_type == "float") return sizeof(float);
        if (glsl_type == "vec2")  return sizeof(float) * 2;
        if (glsl_type == "vec3")  return sizeof(float) * 3;
        if (glsl_type == "vec4")  return sizeof(float) * 4;

        if (glsl_type == "int")   return sizeof(int);
        if (glsl_type == "ivec2") return sizeof(int) * 2;
        if (glsl_type == "ivec3") return sizeof(int) * 3;
        if (glsl_type == "ivec4") return sizeof(int) * 4;

        if (glsl_type == "uint")   return sizeof(uint32_t);
        if (glsl_type == "uvec2")  return sizeof(uint32_t) * 2;
        if (glsl_type == "uvec3")  return sizeof(uint32_t) * 3;
        if (glsl_type == "uvec4")  return sizeof(uint32_t) * 4;

        if (glsl_type == "double") return sizeof(double);
        if (glsl_type == "dvec2")  return sizeof(double) * 2;
        if (glsl_type == "dvec3")  return sizeof(double) * 3;
        if (glsl_type == "dvec4")  return sizeof(double) * 4;

        if (glsl_type == "mat2") return sizeof(float) * 4;
        if (glsl_type == "mat3") return sizeof(float) * 9;
        if (glsl_type == "mat4") return sizeof(float) * 16;

        return 0;
    }


}