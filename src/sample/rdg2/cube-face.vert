#version 450

// Fullscreen triangle from gl_VertexIndex; no vertex buffer needed.
// Vulkan NDC: Y- = visual top, Y+ = visual bottom.

layout(location = 0) out vec2 outUV;

void main() {
    const vec2 kVerts[3] = vec2[](vec2(-1.0, -1.0), vec2(-1.0, 3.0), vec2(3.0, -1.0));
    vec2       pos       = kVerts[gl_VertexIndex];
    gl_Position          = vec4(pos, 0.0, 1.0);
    // NDC [-1,1] -> UV [0,1]. Vulkan Y-down matches standard top-to-bottom texture origin.
    outUV = pos * 0.5 + 0.5;
}
