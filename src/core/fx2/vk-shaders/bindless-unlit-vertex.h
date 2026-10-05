#include "bindless-shared-uniforms.h"
#include "bindless-simple-draw.h"
#include "bindless-simple-material.h"
layout(location = 0) in vec3 position;
#if USE_TEXTURE
layout(location = 2) in vec2 texcoord;
#endif
layout(location = 0) flat out vec4 tint;
layout(location = 1) flat out vec4 emissiveAndCutoff;
layout(location = 2) flat out uvec4 textureIndices;
layout(location = 3) out vec2 uv;
void main() {
    SimpleMaterialData material = materialHeap.materials[draw.materialIndex.x];
    gl_Position                 = sscUniforms.projViewMatrix * draw.worldFromObject * vec4(position, 1);
    tint                        = material.color;
    emissiveAndCutoff           = material.emissiveAndCutoff;
    textureIndices              = material.textureIndices;
    uv                          = vec2(0);
#if USE_TEXTURE
    uv = texcoord;
#endif
}
