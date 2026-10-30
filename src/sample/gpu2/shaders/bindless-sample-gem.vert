#version 450

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;

struct MaterialData {
    vec4  colorTint;
    uint  textureId;
    float shininess;
    vec2  uvScale;
};
layout(std430, set = 0, binding = 0) readonly buffer MaterialBuffer { MaterialData records[]; }
materials;

layout(push_constant) uniform PushConstants {
    mat4 mvp;
    vec4 rotation;
    vec4 lightDir;
    uint materialIndex;
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
    MaterialData material = materials.records[pc.materialIndex];
    float        time     = pc.lightDir.w;
    // Dynamic geometric pulsation along crystal face normal: breathes and expands
    float pulse       = sin(time * 3.5 + in_pos.y * 4.0) * 0.15;
    vec3  animatedPos = in_pos + in_normal * pulse;

    vec4 worldPos = vec4(animatedPos, 1.0);
    gl_Position   = pc.mvp * worldPos;

    out_uv      = in_uv * material.uvScale;
    out_normal  = rotateVector(pc.rotation, in_normal);
    out_tint    = material.colorTint;
    out_texId   = material.textureId;
    out_viewPos = gl_Position.xyz;
    out_pulse   = pulse;
}
