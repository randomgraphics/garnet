#version 450

layout(location = 0) in vec2 in_pos;
layout(location = 1) in vec2 in_uv;

layout(push_constant) uniform PushConstants {
    vec4 transform; // x: scaleX, y: scaleY, z: offsetX, w: offsetY
    uint textureId;
    uint pad[3];
} pc;

layout(location = 0) out vec2 out_uv;

void main() {
    vec2 pos = in_pos * pc.transform.xy + pc.transform.zw;
    gl_Position = vec4(pos, 0.0, 1.0);
    out_uv = in_uv;
}
