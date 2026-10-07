#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec2 inUV;

// Bindless descriptor heap: array of textures at set 0, binding 0
layout(set = 0, binding = 1) uniform texture2D u_textures[];
layout(set = 0, binding = 5) uniform sampler u_samplers[];

layout(push_constant) uniform PushConstants { uint textureIndex; }
pc;

layout(location = 0) out vec4 outColor;

void main() { outColor = texture(sampler2D(u_textures[nonuniformEXT(pc.textureIndex)], u_samplers[0]), inUV); }
