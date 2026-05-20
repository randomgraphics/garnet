#version 450

layout(location = 0) out vec4 color0;
layout(location = 1) out vec4 color1;

void main() {
    color0 = vec4(0.0, 0.0, 1.0, 1.0); // blue  → render target 0
    color1 = vec4(1.0, 0.0, 0.0, 1.0); // red   → render target 1
}
