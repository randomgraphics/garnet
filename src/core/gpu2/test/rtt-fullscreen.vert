#version 450

layout(location = 0) out vec2 outUV;

// Full-screen triangle — no vertex buffer needed.
// CCW in Vulkan's Y-down window space so the triangle is front-facing under
// the default VK_FRONT_FACE_COUNTER_CLOCKWISE without disabling back-face culling.
void main() {
    const vec2 pos[3] = vec2[](vec2(-1.0, -1.0), // top-left
                               vec2(-1.0, 3.0),  // bottom-left (extended)
                               vec2(3.0, -1.0)   // top-right  (extended)
    );
    const vec2 uv[3]  = vec2[](vec2(0.0, 0.0), vec2(0.0, 2.0), vec2(2.0, 0.0));
    outUV             = uv[gl_VertexIndex];
    gl_Position       = vec4(pos[gl_VertexIndex], 0.0, 1.0);
}
