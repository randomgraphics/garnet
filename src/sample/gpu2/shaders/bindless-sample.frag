#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(location = 0) in vec2 in_uv;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec4 in_tint;
layout(location = 3) flat in uint in_texId;
layout(location = 4) in vec3 in_lightDir;
layout(location = 5) in float in_ambient;
layout(location = 6) in float in_shininess;

layout(location = 0) out vec4 out_color;

// Global bindless descriptor heap at Set 0, Binding 0
layout(set = 0, binding = 0) uniform sampler2D u_textures[];

void main() {
    vec4 texColor = texture(u_textures[nonuniformEXT(in_texId)], in_uv);

    // Directional diffuse + Blinn-Phong specular highlight
    vec3  N    = normalize(in_normal);
    vec3  L    = normalize(in_lightDir);
    float diff = max(dot(N, L), 0.0);

    vec3  V    = vec3(0.0, 0.0, 1.0);
    vec3  H    = normalize(L + V);
    float spec = pow(max(dot(N, H), 0.0), in_shininess) * 0.45;

    float lighting = in_ambient + (1.0 - in_ambient) * diff;
    vec3  rgb      = texColor.rgb * in_tint.rgb * lighting + vec3(spec);
    out_color      = vec4(rgb, texColor.a * in_tint.a);
}
