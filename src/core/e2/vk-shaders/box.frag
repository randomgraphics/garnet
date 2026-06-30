#version 450

// Must match the vertex shader / GN::e2::FrameConstants.
layout(std140, set = 0, binding = 0) uniform FrameBlock {
    mat4 viewProj;
    vec4 ambient;
    vec4 lightPosition[4];
    vec4 lightColor[4];
    int  lightCount;
}
u_frame;

layout(location = 0) in vec3 vWorldPos;
layout(location = 1) in vec3 vWorldNormal;
layout(location = 2) in vec3 vColor;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 n   = normalize(vWorldNormal);
    vec3 lit = u_frame.ambient.rgb;

    // Simple Lambert diffuse with photometric inverse-square falloff.
    for (int i = 0; i < u_frame.lightCount; ++i) {
        vec3  toLight = u_frame.lightPosition[i].xyz - vWorldPos;
        float dist2   = max(dot(toLight, toLight), 1e-4);
        vec3  l       = toLight * inversesqrt(dist2);
        float ndotl   = max(dot(n, l), 0.0);
        lit += u_frame.lightColor[i].rgb * (ndotl / dist2);
    }

    outColor = vec4(vColor * lit, 1.0);
}
