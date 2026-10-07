#include "bindless-shared-uniforms.h"
#include "bindless-lambertian-material.h"
layout(push_constant, std430) uniform LambertianDraw {
    mat4  worldFromObject;
    uvec4 materialIndex;
}
draw;
layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
#if USE_TEXTURE
layout(location = 2) in vec2 texcoord;
#endif
layout(location = 0) out vec3 worldNormal;
layout(location = 1) flat out vec4 tint;
layout(location = 2) flat out vec4 emissive;
layout(location = 3) flat out float diffuseMultiplier;
layout(location = 4) flat out uvec4 textureIndices;
layout(location = 5) out vec2 uv;
layout(location = 6) out vec3 worldPosition;
void main() {
    LambertianMaterialData material = materialHeap.materials[draw.materialIndex.x];
    vec4                   p        = draw.worldFromObject * vec4(position, 1);
    gl_Position                     = sscUniforms.projViewMatrix * p;
    worldPosition                   = p.xyz;
    worldNormal                     = transpose(inverse(mat3(draw.worldFromObject))) * normal;
    tint                            = material.color;
    emissive                        = material.emissive;
    diffuseMultiplier               = material.diffuseParameters.x;
    textureIndices                  = material.textureIndices;
    uv                              = vec2(0);
#if USE_TEXTURE
    uv = texcoord;
#endif
}
