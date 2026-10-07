#version 450
#extension GL_GOOGLE_include_directive : require
#extension GL_EXT_nonuniform_qualifier : require
#include "bindless-shared-uniforms.h"
#include "bindless-sky-material.h"

layout(push_constant, std430) uniform PbrDraw {
    mat4  worldFromObject;
    uvec4 materialIndex;
}
draw;

layout(set = 0, binding = 1) uniform texture2D heapTextures[];
layout(set = 0, binding = 1) uniform textureCube heapCubemaps[];
layout(set = 0, binding = 5) uniform sampler heapSamplers[];
vec4 sampleHeap(uint textureIndex, uint samplerIndex, vec2 uv) { return texture(sampler2D(heapTextures[textureIndex], heapSamplers[samplerIndex]), uv); }
layout(location = 0) in vec3 worldNormal;
layout(location = 1) flat in vec4 baseColor;
layout(location = 2) flat in vec4 emissive;
layout(location = 3) flat in vec4 factors;
layout(location = 4) flat in uvec4 textureIndices;
layout(location = 5) flat in uvec4 extraIndices;
layout(location = 6) in vec2 uv;
layout(location = 7) in vec3 worldPosition;
layout(location = 0) out vec4 outColor;
const float PI = 3.14159265359;
vec3        fresnelSchlick(float c, vec3 f0) { return f0 + (1.0 - f0) * pow(clamp(1.0 - c, 0.0, 1.0), 5.0); }
vec3        fresnelSchlickR(float cosTheta, vec3 f0, float roughness) {
    return f0 + (max(vec3(1.0 - roughness), f0) - f0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
void main() {
    uint flags  = extraIndices.z;
    vec4 albedo = baseColor;
    if ((flags & 1u) != 0u) albedo *= sampleHeap(textureIndices.x, extraIndices.y, uv);
    float metallic = factors.x, roughness = factors.y, ao = 1.0;
    vec3  emissiveRadiance = emissive.rgb;
    if ((flags & 4u) != 0u) emissiveRadiance *= sampleHeap(textureIndices.z, extraIndices.y, uv).rgb;
    if ((flags & 8u) != 0u) ao = mix(1.0, sampleHeap(textureIndices.w, extraIndices.y, uv).r, factors.w);
    if ((flags & 16u) != 0u) {
        vec4 mr = sampleHeap(extraIndices.x, extraIndices.y, uv);
        roughness *= mr.g;
        metallic *= mr.b;
    }
    metallic  = clamp(metallic, 0.0, 1.0);
    roughness = clamp(roughness, 0.04, 1.0);
    vec3 n    = normalize(worldNormal);
    if ((flags & 2u) != 0u) {
        vec3  dpdx = dFdx(worldPosition), dpdy = dFdy(worldPosition);
        vec2  duvdx = dFdx(uv), duvdy = dFdy(uv);
        vec3  p1 = cross(dpdy, n), p2 = cross(n, dpdx);
        vec3  t = p1 * duvdx.x + p2 * duvdy.x, b = p1 * duvdx.y + p2 * duvdy.y;
        float scale  = max(dot(t, t), dot(b, b));
        vec3  mapped = sampleHeap(textureIndices.y, extraIndices.y, uv).xyz * 2.0 - 1.0;
        mapped.xy *= factors.z;
        if (scale > 1e-20) n = normalize(mat3(t * inversesqrt(scale), b * inversesqrt(scale), n) * mapped);
    }
    if (!gl_FrontFacing) n = -n;
    vec3 v  = normalize(sscUniforms.cameraPosition.xyz - worldPosition);
    vec3 f0 = mix(vec3(0.04), max(albedo.rgb, vec3(0.0)), metallic), radiance = vec3(0.0);
    for (uint i = 0; i < min(sscUniforms.numLights, uint(MAX_SHARED_LIGHTS)); ++i) {
        DirectLightUniform light           = sscUniforms.lights[i];
        bool               directional     = int(light.positionOrDir.w + 0.5) == 2;
        vec3               delta           = directional ? -light.positionOrDir.xyz : light.positionOrDir.xyz - worldPosition;
        float              distanceToLight = length(delta);
        vec3               l               = delta / max(distanceToLight, 1e-20);
        float              attenuation     = directional ? 1.0 : 1.0 / max(distanceToLight * distanceToLight, 1e-4);
        if (!directional && light.colorAndRange.w > 0.0) attenuation *= pow(clamp(1.0 - distanceToLight / light.colorAndRange.w, 0.0, 1.0), 2.0);
        vec3  h  = normalize(v + l);
        float nl = max(dot(n, l), 0.0), nv = max(dot(n, v), 1e-4), nh = max(dot(n, h), 0.0), vh = max(dot(v, h), 0.0);
        float a = roughness * roughness, a2 = a * a;
        float d        = a2 / max(PI * pow(nh * nh * (a2 - 1.0) + 1.0, 2.0), 1e-6);
        float k        = (roughness + 1.0) * (roughness + 1.0) / 8.0;
        float g        = nv / (nv * (1.0 - k) + k) * nl / (nl * (1.0 - k) + k);
        vec3  f        = fresnelSchlick(vh, f0);
        vec3  specular = d * g * f / max(4.0 * nv * max(nl, 1e-4), 1e-5);
        vec3  diffuse  = (1.0 - f) * (1.0 - metallic) * albedo.rgb / PI;
        radiance += (diffuse + specular) * max(light.colorAndRange.rgb, vec3(0.0)) * attenuation * nl;
    }
    uint skyIdx = draw.materialIndex.y;
    if (skyIdx != 0xFFFFFFFFu) {
        SkyMaterialData sky      = skyMaterialHeap.materials[skyIdx];
        uint            irrTex   = sky.textureIndices.y;
        uint            prefTex  = sky.textureIndices.z;
        uint            brdfTex  = sky.textureIndices.w;
        uint            skySamp  = sky.samplerIndices.x;
        uint            lutSamp  = sky.samplerIndices.y;
        float           lumScale = sky.factors.x;
        float           floorLum = sky.factors.y;

        float nv = max(dot(n, v), 0.0);
        vec3  fr = fresnelSchlickR(nv, f0, roughness);
        vec3  kd = (vec3(1.0) - fr) * (1.0 - metallic);

        vec3 irradiance  = texture(samplerCube(heapCubemaps[irrTex], heapSamplers[skySamp]), n).rgb * lumScale;
        vec3 reflected   = reflect(-v, n);
        vec3 envRadiance = textureLod(samplerCube(heapCubemaps[prefTex], heapSamplers[skySamp]), reflected, roughness * 4.0).rgb * lumScale;
        if (floorLum > 0.0) {
            vec3 floorLighting = vec3(floorLum);
            irradiance         = max(irradiance, floorLighting);
            envRadiance        = max(envRadiance, floorLighting);
        }
        vec2 brdf = texture(sampler2D(heapTextures[brdfTex], heapSamplers[lutSamp]), vec2(nv, roughness)).rg;
        radiance += kd * albedo.rgb * irradiance + envRadiance * (fr * brdf.x + brdf.y);
    }
    vec3 exposed = max(radiance * ao + emissiveRadiance, vec3(0.0)) * max(sscUniforms.exposure, 0.0);
    outColor     = vec4(exposed / (exposed + vec3(1.0)), extraIndices.w != 0u ? 1.0 : albedo.a);
}
