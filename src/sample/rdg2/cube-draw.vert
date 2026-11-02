#version 450
#extension GL_EXT_nonuniform_qualifier : require

layout(std140, set = 1, binding = 1) uniform SharedUniforms {
    uint frameCounter;
    float frameDurationMs;
    vec2 framePadding;
    mat4 viewMatrix;
    mat4 projMatrix;
    mat4 projViewMatrix;
    vec4 cameraPosition;
    vec2 renderTargetSize;
    float nearPlane;
    float farPlane;
    float exposure;
} ssc;

layout(push_constant, std430) uniform CubeDraw { mat4 model; uint cubemapIndex; uint samplerIndex; uint padding0; uint padding1; }
pc;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in float inFaceIndex; // unused; kept so the VBO layout does not change

layout(location = 0) out vec3 outDirection; // object-space position → cubemap sampling direction

void main() {
    gl_Position  = ssc.projViewMatrix * pc.model * vec4(inPosition, 1.0);
    outDirection = inPosition; // cube centered at origin; vertex position = sample direction
}
