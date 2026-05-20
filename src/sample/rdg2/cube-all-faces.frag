#version 450

// One texture per cube face (impl 2: 6 MRT color targets in a single render pass).
// Each binding maps to one face image loaded from disk; each output location maps to
// a GpuRaster color-target slot. The vertex shader draws a fullscreen triangle.
layout(set = 0, binding = 0) uniform sampler2D uFace0; // +X
layout(set = 0, binding = 1) uniform sampler2D uFace1; // -X
layout(set = 0, binding = 2) uniform sampler2D uFace2; // +Y
layout(set = 0, binding = 3) uniform sampler2D uFace3; // -Y
layout(set = 0, binding = 4) uniform sampler2D uFace4; // +Z
layout(set = 0, binding = 5) uniform sampler2D uFace5; // -Z

layout(location = 0) in vec2 inUV;

layout(location = 0) out vec4 outFace0;
layout(location = 1) out vec4 outFace1;
layout(location = 2) out vec4 outFace2;
layout(location = 3) out vec4 outFace3;
layout(location = 4) out vec4 outFace4;
layout(location = 5) out vec4 outFace5;

void main() {
    outFace0 = texture(uFace0, inUV);
    outFace1 = texture(uFace1, inUV);
    outFace2 = texture(uFace2, inUV);
    outFace3 = texture(uFace3, inUV);
    outFace4 = texture(uFace4, inUV);
    outFace5 = texture(uFace5, inUV);
}
