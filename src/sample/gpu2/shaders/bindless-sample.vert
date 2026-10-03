#version 450

layout(location = 0) in vec3 in_pos;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_uv;

layout(push_constant) uniform PushConstants {
    mat4  mvp;       // 64 bytes: Model-View-Projection matrix
    vec4  rotation;  // 16 bytes: orientation quaternion (qx, qy, qz, qw)
    vec4  colorTint; // 16 bytes: RGBA color multiplier
    uint  textureId; // 4 bytes: bindless descriptor slot in Set 0
    float shininess; // 4 bytes: specular exponent
    vec2  uvScale;   // 8 bytes: UV coordinate scaling
    vec4  lightDir;  // 16 bytes: normalized light dir (xyz) + ambient intensity (w)
}
pc;

layout(location = 0) out vec2 out_uv;
layout(location = 1) out vec3 out_normal;
layout(location = 2) out vec4 out_tint;
layout(location = 3) flat out uint out_texId;
layout(location = 4) out vec3 out_lightDir;
layout(location = 5) out float out_ambient;
layout(location = 6) out float out_shininess;

vec3 rotateVector(vec4 q, vec3 v) { return v + 2.0 * cross(q.xyz, cross(q.xyz, v) + q.w * v); }

void main() {
    gl_Position   = pc.mvp * vec4(in_pos, 1.0);
    out_uv        = in_uv * pc.uvScale;
    out_normal    = rotateVector(pc.rotation, in_normal);
    out_tint      = pc.colorTint;
    out_texId     = pc.textureId;
    out_lightDir  = pc.lightDir.xyz;
    out_ambient   = pc.lightDir.w;
    out_shininess = pc.shininess;
}
