#version 450

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;

layout(push_constant) uniform PushConstants {
    mat4  mvp;       // 64 bytes: Model-View-Projection matrix
    vec4  rotation;  // 16 bytes: orientation quaternion (qx, qy, qz, qw)
    vec4  colorTint; // 16 bytes: RGBA color multiplier
    uint  textureId; // 4 bytes: bindless descriptor slot in Set 0
    float shininess; // 4 bytes: effect parameter / pulse frequency
    vec2  uvScale;   // 8 bytes: UV coordinate scaling
    vec4  lightDir;  // 16 bytes: normalized light dir (xyz) + time/phase (w)
}
pc;

layout(location = 0) out vec2 out_uv;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec4 out_tint;
layout(location = 3) flat out uint out_texId;
layout(location = 4) out vec3 out_viewPos;
layout(location = 5) out float out_pulse;

vec3 rotateVector(vec4 q, vec3 v) { return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v); }

void main() {
    float time = pc.lightDir.w;
    // Dynamic geometric pulsation along crystal face normal: breathes and expands
    float pulse       = sin(time * 3.5 + in_pos.y * 4.0) * 0.15;
    vec3  animatedPos = in_pos + in_normal * pulse;

    vec4 worldPos = vec4(animatedPos, 1.0);
    gl_Position   = pc.mvp * worldPos;

    out_uv      = in_uv * pc.uvScale;
    out_normal  = rotateVector(pc.rotation, in_normal);
    out_tint    = pc.colorTint;
    out_texId   = pc.textureId;
    out_viewPos = gl_Position.xyz;
    out_pulse   = pulse;
}
