#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec4 inColor;
layout(location = 1) in vec2 inUV;
layout(location = 2) flat in uint inAlbedo;

layout(location = 0) out vec4 outColor;

layout(set = 0, binding = 0) uniform sampler2D textures[];

void main() {
    vec4 tex = texture(textures[inAlbedo], inUV);
    outColor = inColor * tex;
}

