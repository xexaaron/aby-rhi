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
layout(location = 3) flat out uint outNormal;

void main() {
    uint instance = gl_InstanceIndex;

    const float spacing = 1.5;

    float x = float(instance % 3u) - 1.0;
    float y = float((instance / 3u) % 3u) - 1.0;
    float z = float(instance / 9u) - 1.0;

    vec3 offset = vec3(x, y, z) * spacing;

    vec4 world_position =
        mvp.model * vec4(inPosition, 1.0);

    world_position.xyz += offset;

    gl_Position =
        mvp.projection *
        mvp.view *
        world_position;

    outColor = inColor;
    outUV = inUV;
    outAlbedo = texs.albedo;
    outNormal = texs.normal;
}