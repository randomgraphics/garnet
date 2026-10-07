#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_nonuniform_qualifier : require
#include "bindless-shared-uniforms.h"
#include "bindless-cel-material.h"

layout(push_constant, std430) uniform CelDraw {
    mat4  worldFromObject;
    uvec4 materialIndex;
}
draw;

layout(set = 0, binding = 1) uniform texture2D heapTextures[];
layout(set = 0, binding = 5) uniform sampler heapSamplers[];

vec4 sampleHeap(uint textureIndex, uint samplerIndex, vec2 uv) { return texture(sampler2D(heapTextures[textureIndex], heapSamplers[samplerIndex]), uv); }

layout(location = 0) in vec3 worldNormal;
layout(location = 1) in vec2 uv;
layout(location = 2) in vec3 worldPosition;

layout(location = 0) out vec4 outColor;

void main() {
    CelMaterialData material  = celMaterialHeap.materials[draw.materialIndex.x];
    vec4            baseColor = material.baseColor;
    if ((material.textureIndices.w & 1u) != 0u) { baseColor *= sampleHeap(material.textureIndices.x, material.textureIndices.z, uv); }

    vec3 n = worldNormal / max(length(worldNormal), 1e-20);
    if ((material.textureIndices.w & 2u) != 0u) {
        vec3  dpdx = dFdx(worldPosition), dpdy = dFdy(worldPosition);
        vec2  duvdx = dFdx(uv), duvdy = dFdy(uv);
        vec3  p1 = cross(dpdy, n), p2 = cross(n, dpdx);
        vec3  t      = p1 * duvdx.x + p2 * duvdy.x;
        vec3  b      = p1 * duvdx.y + p2 * duvdy.y;
        float scale  = max(dot(t, t), dot(b, b));
        vec3  mapped = sampleHeap(material.textureIndices.y, material.textureIndices.z, uv).xyz * 2.0 - 1.0;
        if (scale > 1e-20) n = normalize(mat3(t * inversesqrt(scale), b * inversesqrt(scale), n) * mapped);
    }
    if (!gl_FrontFacing) n = -n;

    vec3  v     = normalize(sscUniforms.cameraPosition.xyz - worldPosition);
    float ndotv = max(dot(n, v), 0.0);

    vec3  diffuseAccum  = vec3(0.0);
    vec3  specularAccum = vec3(0.0);
    float mainShadow    = 1.0;

    for (uint i = 0u; i < min(sscUniforms.numLights, uint(MAX_SHARED_LIGHTS)); ++i) {
        DirectLightUniform light     = sscUniforms.lights[i];
        uint               type      = uint(light.positionOrDir.w + 0.5);
        vec3               delta     = light.positionOrDir.xyz - worldPosition;
        float              distance2 = max(dot(delta, delta), 0.0001);
        vec3               l =
            type == SHARED_LIGHT_TYPE_DIRECTIONAL ? -light.positionOrDir.xyz / max(length(light.positionOrDir.xyz), 1e-20) : delta * inversesqrt(distance2);
        float attenuation = type == SHARED_LIGHT_TYPE_DIRECTIONAL ? 1.0 : 1.0 / distance2;
        if (type != SHARED_LIGHT_TYPE_DIRECTIONAL && light.colorAndRange.w > 0.0) {
            attenuation *= pow(clamp(1.0 - sqrt(distance2) / light.colorAndRange.w, 0.0, 1.0), 2.0);
        }

        vec3 lightColor = max(light.colorAndRange.rgb, vec3(0.0));

        // 1. Multi-band cartoon diffuse (Half-Lambert wrap + quantized shadow ramps)
        float ndotl             = dot(n, l);
        float halfLambert       = clamp(0.5 * ndotl + 0.5, 0.0, 1.0);
        float shadow1           = smoothstep(material.shadowParams.x - material.shadowParams.y, material.shadowParams.x + material.shadowParams.y, halfLambert);
        float shadow2           = smoothstep(material.shadowParams.z - material.shadowParams.w, material.shadowParams.z + material.shadowParams.w, halfLambert);
        vec3  shadowColor       = mix(material.deepShadowTint.rgb, material.shadowTint.rgb, shadow2);
        vec3  diffuseMultiplier = mix(shadowColor, vec3(1.0), shadow1);
        diffuseAccum += baseColor.rgb * diffuseMultiplier * lightColor * attenuation;

        // 2. Stepped Anime Specular
        vec3  h          = normalize(l + v);
        float ndoth      = max(dot(n, h), 0.0);
        float specLobe   = pow(ndoth, max(material.specularParams.y, 1.0));
        float specFactor = smoothstep(material.specularParams.x - 0.02, material.specularParams.x + 0.02, specLobe);
        specularAccum += specFactor * material.specularParams.z * lightColor * attenuation * shadow1;

        if (i == 0u) { mainShadow = shadow1; }
    }

    // 3. Stylized Rim Light (Fresnel silhouette glow)
    float fresnel   = 1.0 - ndotv;
    float rimVal    = pow(clamp(fresnel, 0.0, 1.0), 3.0);
    float rimFactor = smoothstep(material.rimParams.x - material.rimParams.y, material.rimParams.x + material.rimParams.y, rimVal);
    rimFactor *= clamp(mainShadow + 0.3, 0.0, 1.0);
    vec3 rim = rimFactor * material.rimParams.z * material.rimTint.rgb;

    // 4. Soft ambient
    vec3 ambient = baseColor.rgb * 0.05;

    // 5. Combine & Reinhard tone mapping with exposure
    vec3 radiance = diffuseAccum + specularAccum + rim + ambient + material.emissive.rgb;
    vec3 exposed  = max(radiance, vec3(0.0)) * max(sscUniforms.exposure, 0.0);
    outColor      = vec4(exposed / (exposed + vec3(1.0)), baseColor.a);
}
