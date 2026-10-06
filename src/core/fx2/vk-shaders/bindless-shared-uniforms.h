// Mirrors public SharedUniforms in bindless/shared-shader-constants.h (1024 bytes, std140).
// Legacy frame/light and camera data share one block; only sky-material selection remains.
#define MAX_SHARED_LIGHTS             16
#define SHARED_LIGHT_TYPE_POINT       0u
#define SHARED_LIGHT_TYPE_SPOT        1u
#define SHARED_LIGHT_TYPE_DIRECTIONAL 2u
struct DirectLightUniform {
    vec4 positionOrDir;
    vec4 colorAndRange;
    vec4 coneAngles;
};
layout(set = 1, binding = 1, std140) uniform SharedUniformBlock {
    uint               frameCounter;
    float              frameDurationMs;
    vec2               framePadding;
    mat4               viewMatrix;
    mat4               projMatrix;
    mat4               projViewMatrix;
    vec4               cameraPosition;
    vec2               renderTargetSize;
    float              nearPlane;
    float              farPlane;
    float              exposure;
    uint               numLights;
    DirectLightUniform lights[MAX_SHARED_LIGHTS];
}
sscUniforms;
