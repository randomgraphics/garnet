#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec2 in_uv;
layout(location = 0) out vec4 out_color;

layout(set = 0, binding = 0) uniform sampler2D u_textures[];

layout(push_constant) uniform PushConstants {
    vec4 transform;
    uint textureId;
    uint pad[3];
}
pc;

void main() { out_color = texture(u_textures[nonuniformEXT(pc.textureId)], in_uv); }
