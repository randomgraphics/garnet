#extension GL_GOOGLE_include_directive : require

#include "camera-ubo.h"
#include "cel-shading-ubo.h"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
#if (LIT_INPUTS & 1)
layout(location = 2) in vec2 inTexCoord;
#else
const vec2 inTexCoord = vec2(0);
#endif
#if (LIT_INPUTS & 2)
layout(location = 3) in vec4 inTangent;
#else
const vec4 inTangent = vec4(1, 0, 0, 1);
#endif
#if (LIT_INPUTS & 4)
layout(location = 4) in vec4 inColor;
#else
const vec4 inColor = vec4(1);
#endif

layout(location = 0) out vec3 outWorldPos;
layout(location = 1) out vec3 outNormal;
#if (LIT_INPUTS & 1)
layout(location = 2) out vec2 outTexCoord;
#endif
#if (LIT_INPUTS & 4)
layout(location = 3) out vec4 outColor;
#endif
#if (LIT_INPUTS & 2)
layout(location = 4) out vec4 outTangent;
#endif

layout(push_constant, std430) uniform PC {
    mat4 worldTransform;
    mat4 normalTransform;
}
pc;

void main() {
    vec4 worldPos = pc.worldTransform * vec4(inPosition, 1.0);
    worldPos.xyz += u_cel.outlineParams.xyz * 1e-9;
    outWorldPos = worldPos.xyz;
    outNormal   = mat3(pc.normalTransform) * inNormal;
#if (LIT_INPUTS & 2)
    outTangent = vec4(mat3(pc.worldTransform) * inTangent.xyz, inTangent.w * sign(determinant(mat3(pc.worldTransform))));
#endif
#if (LIT_INPUTS & 1)
    outTexCoord = inTexCoord;
#endif
#if (LIT_INPUTS & 4)
    outColor = inColor;
#endif
    gl_Position = u_camera.projViewMatrix * worldPos;
}
