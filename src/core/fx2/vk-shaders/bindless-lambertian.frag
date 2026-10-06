#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_nonuniform_qualifier : require
#include "bindless-shared-uniforms.h"
layout(set = 0, binding = 1) uniform texture2D heapTextures[];
layout(set = 0, binding = 5) uniform sampler heapSamplers[];
vec4 sampleHeap(uint textureIndex, uint samplerIndex, vec2 uv) {
    return texture(sampler2D(heapTextures[nonuniformEXT(textureIndex)], heapSamplers[nonuniformEXT(samplerIndex)]), uv);
}
layout(location = 0) in vec3 worldNormal;
layout(location = 1) flat in vec4 tint;
layout(location = 2) flat in vec4 emissive;
layout(location = 3) flat in float diffuseMultiplier;
layout(location = 4) flat in uvec4 textureIndices;
layout(location = 5) in vec2 uv;
layout(location = 6) in vec3 worldPosition;
layout(location = 0) out vec4 outColor;
void main() {
    // Derivatives are evaluated before branches for the tangent frame.
    vec3 dpdx = dFdx(worldPosition), dpdy = dFdy(worldPosition);
    vec2 duvdx = dFdx(uv), duvdy = dFdy(uv);
    vec4 color = tint;
    if ((textureIndices.w & 1u) != 0u) color *= sampleHeap(textureIndices.x, textureIndices.z, uv);
    vec3 n = worldNormal / max(length(worldNormal), 1e-20);
    if ((textureIndices.w & 2u) != 0u) {
        // A cotangent frame handles mirrored UVs and nonuniform world scale without stored tangents.
        vec3  p1 = cross(dpdy, n), p2 = cross(n, dpdx);
        vec3  t      = p1 * duvdx.x + p2 * duvdy.x;
        vec3  b      = p1 * duvdx.y + p2 * duvdy.y;
        float scale  = max(dot(t, t), dot(b, b));
        vec3  mapped = sampleHeap(textureIndices.y, textureIndices.z, uv).xyz * 2 - 1;
        if (scale > 1e-20) n = normalize(mat3(t * inversesqrt(scale), b * inversesqrt(scale), n) * mapped);
    }
    if (!gl_FrontFacing) n = -n;
    vec3 irradiance = vec3(0);
    for (uint i = 0u; i < min(sscUniforms.numLights, uint(MAX_SHARED_LIGHTS)); ++i) {
        DirectLightUniform light     = sscUniforms.lights[i];
        uint               type      = uint(light.positionOrDir.w + 0.5);
        vec3               delta     = light.positionOrDir.xyz - worldPosition;
        float              distance2 = max(dot(delta, delta), 0.0001);
        vec3               direction =
            type == SHARED_LIGHT_TYPE_DIRECTIONAL ? -light.positionOrDir.xyz / max(length(light.positionOrDir.xyz), 1e-20) : delta * inversesqrt(distance2);
        float attenuation = type == SHARED_LIGHT_TYPE_DIRECTIONAL ? 1.0 : 1.0 / distance2;
        if (type != SHARED_LIGHT_TYPE_DIRECTIONAL && light.colorAndRange.w > 0.0) {
            attenuation *= pow(clamp(1.0 - sqrt(distance2) / light.colorAndRange.w, 0.0, 1.0), 2.0);
        }
        // Match legacy Lambertian: SPOT records currently use point-light attenuation.
        irradiance += max(light.colorAndRange.rgb, vec3(0)) * max(dot(n, direction), 0.0) * attenuation;
    }
    // Sky material sampling belongs to the future sky kernel; no fixed environment bindings remain.
    vec3 radiance = max(color.rgb, vec3(0)) * irradiance * diffuseMultiplier / 3.14159265359 + emissive.rgb;
    vec3 exposed  = max(radiance, vec3(0)) * max(sscUniforms.exposure, 0);
    outColor      = vec4(exposed / (exposed + vec3(1)), color.a);
}
