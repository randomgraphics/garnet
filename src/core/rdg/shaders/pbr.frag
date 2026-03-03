#version 450

/// Minimal PBR fragment shader (metallic-roughness).
/// Task 3.2 will add uniforms for light and material (baseColor, metallic, roughness).

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
    vec3 N = normalize(inNormal);
    // Placeholder: one directional light from above
    vec3 L = normalize(vec3(0.2, -1.0, 0.3));
    float NdotL = max(dot(N, L), 0.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 diffuse = lightColor * NdotL;
    vec3 baseColor = vec3(0.8, 0.8, 0.8);
    vec3 color = baseColor * (0.15 + diffuse);
    outColor = vec4(color, 1.0);
}
