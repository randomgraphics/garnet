#include "bindless-shared-uniforms.h"
#include "bindless-cel-material.h"

layout(push_constant, std430) uniform CelDraw {
    mat4  worldFromObject;
    uvec4 materialIndex;
}
draw;

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;
#if USE_TEXTURE
layout(location = 2) in vec2 texcoord;
#endif

layout(location = 0) out vec2 uv;

void main() {
    CelMaterialData material    = celMaterialHeap.materials[draw.materialIndex.x];
    vec4            worldPos    = draw.worldFromObject * vec4(position, 1.0);
    vec3            worldNormal = transpose(inverse(mat3(draw.worldFromObject))) * normal;

    vec4 clipPos = sscUniforms.projViewMatrix * worldPos;

    // Transform world normal to view space, then project to clip space
    vec3 viewNormal = mat3(sscUniforms.viewMatrix) * worldNormal;
    vec2 clipNormal = mat2(sscUniforms.projMatrix) * viewNormal.xy;

    float normalLen = length(clipNormal);
    vec2  normalDir = normalLen > 1e-5 ? clipNormal / normalLen : vec2(0.0);

    float aspect = sscUniforms.renderTargetSize.y > 0.0 ? (sscUniforms.renderTargetSize.x / sscUniforms.renderTargetSize.y) : 1.0;
    vec2  offset = vec2(normalDir.x / aspect, normalDir.y) * material.outlineParams.x * clipPos.w;

    clipPos.xy += offset;
    gl_Position = clipPos;

    uv = vec2(0.0);
#if USE_TEXTURE
    uv = texcoord;
#endif
}
