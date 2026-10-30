#version 450
#extension GL_GOOGLE_include_directive : require

#include "bindless-shared-uniforms.h"

layout(location = 0) out vec3 v_dir;

// Full-screen triangle; no VBO needed.
const vec2 kPos[3] = vec2[](vec2(-1.0, -1.0), vec2(3.0, -1.0), vec2(-1.0, 3.0));

void main() {
    vec2 ndc = kPos[gl_VertexIndex];

    // Unproject NDC to world-space direction using the shared camera matrices.
    mat4 invProj    = inverse(sscUniforms.projMatrix);
    mat3 invViewRot = transpose(mat3(sscUniforms.viewMatrix));

    vec4 viewDir = invProj * vec4(ndc, 1.0, 1.0);
    viewDir.z    = -1.0;

    v_dir = invViewRot * viewDir.xyz;

    // Depth = 1.0 so skybox is always behind geometry (LESS_EQUAL depth test).
    gl_Position = vec4(ndc, 1.0, 1.0);
}
