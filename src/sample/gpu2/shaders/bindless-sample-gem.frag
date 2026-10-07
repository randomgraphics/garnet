#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec2 in_uv;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec4 in_tint;
layout(location = 3) flat in uint in_texId;
layout(location = 4) in vec3 in_viewPos;
layout(location = 5) in float in_pulse;

layout(location = 0) out vec4 out_color;

// Typed arrays share set 0 with the material SSBO at binding 0
layout(set = 0, binding = 1) uniform texture2D u_textures[];
layout(set = 0, binding = 5) uniform sampler u_samplers[];

void main() {
    // Holographic chromatic dispersion: sample bindless texture with R/G/B phase shift
    vec2  disp   = vec2(0.015, -0.010) * (1.0 + in_pulse * 2.0);
    float r      = texture(sampler2D(u_textures[nonuniformEXT(in_texId)], u_samplers[0]), in_uv + disp).r;
    float g      = texture(sampler2D(u_textures[nonuniformEXT(in_texId)], u_samplers[0]), in_uv).g;
    float b      = texture(sampler2D(u_textures[nonuniformEXT(in_texId)], u_samplers[0]), in_uv - disp).b;
    vec3  texRgb = vec3(r, g, b);

    // Fresnel rim glow: glowing edges
    vec3  N       = normalize(in_normal);
    vec3  V       = vec3(0.0, 0.0, 1.0);
    float NdotV   = abs(dot(N, V));
    float fresnel = pow(1.0 - NdotV, 2.5);

    // Glowing energy core + scanline oscillation
    float scanline  = sin(in_viewPos.y * 70.0) * 0.12;
    vec3  glowColor = in_tint.rgb * 1.6 + vec3(0.35, 0.55, 0.85);
    vec3  finalRgb  = texRgb * in_tint.rgb * 0.65 + glowColor * (fresnel + scanline);

    out_color = vec4(finalRgb, 1.0);
}
