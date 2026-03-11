#version 450

/// Minimal PBR vertex shader: position, normal, UV.
/// Uniforms (set in Task 3.2): model, viewProj.

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec2 outTexCoord;

layout(push_constant, std430) uniform Constants {
    mat4 model;
    mat4 viewProj;
}
pc;

void main() {
    vec4 worldPos = pc.model * vec4(inPosition, 1.0);
    outWorldPos   = worldPos.xyz;
    outNormal     = mat3(transpose(inverse(pc.model))) * inNormal;
    outTexCoord   = inTexCoord;
    gl_Position   = pc.viewProj * worldPos;
}
