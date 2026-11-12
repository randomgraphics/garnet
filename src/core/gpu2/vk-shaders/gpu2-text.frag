#version 450

layout(push_constant) uniform TextPushConstants {
    vec2 origin;
    vec2 glyphSize;
    vec2 viewportOrigin;
    vec2 viewportSize;
    vec4 color;
    uvec2 bitmap;
} pc;

layout(location = 0) in vec2 inGlyphUv;
layout(location = 1) flat in vec4 inColor;
layout(location = 0) out vec4 outColor;

void main() {
    ivec2 cell = clamp(ivec2(floor(inGlyphUv * vec2(5.0, 7.0))), ivec2(0), ivec2(4, 6));
    uint bitIndex = uint(cell.y * 5 + cell.x);
    uint bitValue = bitIndex < 32u ? ((pc.bitmap.x >> bitIndex) & 1u) : ((pc.bitmap.y >> (bitIndex - 32u)) & 1u);
    if (bitValue == 0u) discard;
    outColor = inColor;
}
