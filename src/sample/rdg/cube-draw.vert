#version 450

layout(push_constant) uniform PC { mat4 mvp; }
pc;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in float inFaceIndex; // unused; kept so the vertex buffer layout does not change

layout(location = 0) out vec3 outDirection; // object-space position → cubemap sampling direction

void main() {
    gl_Position  = pc.mvp * vec4(inPosition, 1.0);
    outDirection = inPosition; // cube is centered at origin, so vertex position = sample direction
}
