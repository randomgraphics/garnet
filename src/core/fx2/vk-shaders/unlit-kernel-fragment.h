#if USE_TEXTURE
layout(set = 1, binding = 0) uniform sampler2D colorMap;
#endif
layout(location = 0) in vec2 uv;
layout(location = 1) in vec4 tint;
layout(location = 2) flat in vec4 emissiveAndCutoff;
layout(location = 0) out vec4 outputColor;
void main() {
    vec4 color = tint;
#if USE_TEXTURE
    color *= texture(colorMap, uv);
#endif
    if (color.a < emissiveAndCutoff.w) discard;
    outputColor = vec4(color.rgb + emissiveAndCutoff.rgb, color.a);
}
