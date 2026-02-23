#version 450

/// Isosceles triangle at screen center; width and height ~1/3 of screen in NDC.
void main() {
    float h           = 1.0 / 3.0;           // half extent in NDC (~1/3 of screen)
    vec2  vertices[3] = vec2[](vec2(0.0, h), // top (apex)
                              vec2(-h, -h), // bottom-left
                              vec2(h, -h)   // bottom-right
    );
    gl_Position       = vec4(vertices[gl_VertexIndex], 0.0, 1.0);
}
