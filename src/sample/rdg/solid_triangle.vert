#version 450

/// Full-screen triangle via gl_VertexIndex; NDC positions (-1,-1), (3,-1), (-1,3).
void main() {
    vec2 vertices[3] = vec2[](vec2(-1.0, -1.0), vec2(3.0, -1.0), vec2(-1.0, 3.0));
    gl_Position = vec4(vertices[gl_VertexIndex], 0.0, 1.0);
}
