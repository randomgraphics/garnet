#version 450
#extension GL_GOOGLE_include_directive : require

#include "camera-ubo.h"
#include "cel-shading-ubo.h"

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec4 inTangent;
layout(location = 4) in vec4 inColor;

layout(location = 0) out vec2 outTexCoord;
layout(location = 1) out vec4 outColor;

layout(push_constant, std430) uniform PC {
    mat4 worldTransform;
    mat4 normalTransform;
}
pc;

void main() {
    vec4 worldPos = pc.worldTransform * vec4(inPosition, 1.0);
    vec3 worldNormal = normalize(mat3(pc.normalTransform) * inNormal);

    vec4 clipPos = u_camera.projViewMatrix * worldPos;

    // Transform world normal to view space, then project to clip space
    vec3 viewNormal = mat3(u_camera.viewMatrix) * worldNormal;
    vec2 clipNormal = mat2(u_camera.projMatrix) * viewNormal.xy;

    float normalLen = length(clipNormal);
    vec2 normalDir = normalLen > 1e-5 ? clipNormal / normalLen : vec2(0.0);

    float aspect = u_camera.renderTargetSize.y > 0.0 ? (u_camera.renderTargetSize.x / u_camera.renderTargetSize.y) : 1.0;
    vec2 offset = vec2(normalDir.x / aspect, normalDir.y) * u_cel.outlineParams.x * clipPos.w;

    clipPos.xy += offset;

    outTexCoord = inTexCoord;
    outColor    = inColor;
    gl_Position = clipPos;
}
