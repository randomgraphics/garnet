#version 450

/// Isosceles triangle at screen center; width and height ~1/3 of screen in NDC.
void main() {
    float h = 1.0 / 3.0; // half extent in NDC (~1/3 of screen)
    // Vulkan NDC: Y- = visual top, Y+ = visual bottom. CCW winding (front-face).
    vec2 vertices[3] = vec2[](vec2(0.0, -h), // apex (visual top)
                              vec2(-h, h),   // visual bottom-left
                              vec2(h, h)     // visual bottom-right
    );
    gl_Position      = vec4(vertices[gl_VertexIndex], 0.0, 1.0);
}
