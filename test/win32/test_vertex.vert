#version 450
#extension GL_EXT_nonuniform_qualifier : require
#extension GL_EXT_debug_printf : enable

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;
layout(location = 2) in vec4 inColor;

layout(set = 1, binding = 0) uniform MVP {
    mat4 model;
    mat4 view;
    mat4 projection;
} mvp;

layout(set = 1, binding = 1) uniform TEXTURES {
    uint albedo;
    uint ao;
    uint height;
    uint normal;
    uint roughness;
    uint orm;
} texs;

layout(location = 0) out vec4 outColor;
layout(location = 1) out vec2 outUV;
layout(location = 2) flat out uint outAlbedo;

void main() {
    gl_Position = mvp.projection * mvp.view * mvp.model * vec4(inPosition, 1.0);
    outColor = inColor;
    outUV = inUV;
    outAlbedo = texs.albedo;
}