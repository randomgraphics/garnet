#version 450
#extension GL_GOOGLE_include_directive : require

#include "global-camera-ubo.h"
layout(std140, set = 0, binding = 0) uniform GlobalCameraBlock { GlobalCameraUBO data; } u_camera;

layout(location = 0) out vec3 v_dir;

// Full-screen triangle (3 verts, no VBO).
// NDC positions for vertexIndex 0,1,2: covers [-1,1] × [-1,1].
const vec2 kPositions[3] = vec2[](
    vec2(-1.0, -1.0),
    vec2( 3.0, -1.0),
    vec2(-1.0,  3.0)
);

void main() {
    vec2 ndc = kPositions[gl_VertexIndex];

    // Reconstruct the view-space ray direction (no translation, pure rotation).
    // Invert projection to get view-space direction, then rotate to world-space.
    mat4 invProj    = inverse(u_camera.data.projMatrix);
    mat3 invViewRot = transpose(mat3(u_camera.data.viewMatrix)); // rotation inverse = transpose

    // Unproject NDC to a view-space direction; discard translation (w = 0).
    vec4 viewDir = invProj * vec4(ndc, 1.0, 1.0);
    viewDir.z = -1.0; // point "into" the screen (forward = -Z)

    v_dir = invViewRot * viewDir.xyz;

    // Place at maximum depth so skybox is always behind geometry.
    gl_Position = vec4(ndc, 1.0, 1.0); // z/w == 1.0 == far plane depth
}
