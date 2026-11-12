#version 450

layout(push_constant) uniform TextPushConstants {
    vec2  origin;
    vec2  glyphSize;
    vec2  viewportOrigin;
    vec2  viewportSize;
    vec4  color;
    uvec2 bitmap;
}
pc;

layout(location = 0) out vec2 outGlyphUv;
layout(location = 1) flat out vec4 outColor;

void main() {
    uint       vertex     = uint(gl_VertexIndex);
    uint       corner     = vertex % 6u;
    const vec2 corners[6] = vec2[6](vec2(0, 0), vec2(1, 0), vec2(1, 1), vec2(0, 0), vec2(1, 1), vec2(0, 1));
    outGlyphUv            = corners[corner];
    outColor              = pc.color;
    vec2 pixel            = pc.origin + outGlyphUv * vec2(pc.glyphSize.x * (5.0 / 6.0), pc.glyphSize.y);
    vec2 ndc              = (pixel - pc.viewportOrigin) / pc.viewportSize * 2.0 - 1.0;
    gl_Position           = vec4(ndc, 0.0, 1.0);
}
