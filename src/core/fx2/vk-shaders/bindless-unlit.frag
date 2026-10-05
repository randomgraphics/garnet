#version 450
#extension GL_GOOGLE_include_directive : require
#include "bindless-simple-textures.h"
layout(location = 0) flat in vec4 tint;
layout(location = 1) flat in vec4 emissiveAndCutoff;
layout(location = 2) flat in uvec4 textureIndices;
layout(location = 3) in vec2 uv;
layout(location = 0) out vec4 outColor;
void main() {
    vec4 color = tint;
    if ((textureIndices.w & 1u) != 0u) color *= sampleHeap(textureIndices.x, textureIndices.z, uv);
    if (color.a < emissiveAndCutoff.w) discard;
    outColor = vec4(color.rgb + emissiveAndCutoff.rgb, color.a);
}
