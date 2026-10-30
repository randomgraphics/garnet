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

layout(location = 0) out vec3 worldNormal;
layout(location = 1) out vec2 uv;
layout(location = 2) out vec3 worldPosition;

void main() {
    vec4 p        = draw.worldFromObject * vec4(position, 1.0);
    gl_Position   = sscUniforms.projViewMatrix * p;
    worldPosition = p.xyz;
    worldNormal   = transpose(inverse(mat3(draw.worldFromObject))) * normal;
    uv            = vec2(0.0);
#if USE_TEXTURE
    uv = texcoord;
#endif
}
