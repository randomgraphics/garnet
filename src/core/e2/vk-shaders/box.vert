#version 450

// Per-frame constants (camera + lights). set = 0, binding = 0.
// Field order/layout must match GN::e2::FrameConstants in e2-internal.h.
layout(std140, set = 0, binding = 0) uniform FrameBlock {
    mat4 viewProj;
    vec4 ambient;
    vec4 lightPosition[4];
    vec4 lightColor[4];
    int  lightCount;
}
u_frame;

// Per-draw constants. Must match GN::e2::DrawConstants.
layout(push_constant) uniform DrawBlock {
    mat4 model;
    vec4 baseColor;
}
pc;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;

layout(location = 0) out vec3 vWorldPos;
layout(location = 1) out vec3 vWorldNormal;
layout(location = 2) out vec3 vColor;

void main() {
    vec4 worldPos = pc.model * vec4(inPosition, 1.0);
    vWorldPos     = worldPos.xyz;
    // Approximate normal transform with the model's upper-left 3x3. Adequate for the
    // near-uniform box scales this simple world uses.
    vWorldNormal = mat3(pc.model) * inNormal;
    vColor       = pc.baseColor.rgb;
    gl_Position  = u_frame.viewProj * worldPos;
}
