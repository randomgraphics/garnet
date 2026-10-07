#version 450

layout(location = 0) out vec2 outUV;

// Full-screen triangle for bindless raster testing
void main() {
    const vec2 pos[3] = vec2[](vec2(-1.0, -1.0), vec2(-1.0, 3.0), vec2(3.0, -1.0));
    const vec2 uv[3]  = vec2[](vec2(0.0, 0.0), vec2(0.0, 2.0), vec2(2.0, 0.0));
    outUV             = uv[gl_VertexIndex];
    gl_Position       = vec4(pos[gl_VertexIndex], 0.0, 1.0);
}
