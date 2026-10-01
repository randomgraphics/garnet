#extension GL_GOOGLE_include_directive : require

#include "scene-ubo.h"
#include "camera-ubo.h"
#include "model-material-ubo.h"
#include "cel-shading-ubo.h"

layout(set = 1, binding = 0) uniform sampler2D u_baseColor;
layout(set = 1, binding = 1) uniform sampler2D u_normal;
layout(set = 1, binding = 2) uniform sampler2D u_emissive;
layout(set = 1, binding = 3) uniform sampler2D u_occlusion;
layout(set = 1, binding = 4) uniform sampler2D u_metalRough;

// Match the vertex variant: absent features need no interpolator or VS output.
layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
#if (LIT_INPUTS & 1)
layout(location = 2) in vec2 inTexCoord;
#else
const vec2 inTexCoord = vec2(0);
#endif
#if (LIT_INPUTS & 4)
layout(location = 3) in vec4 inColor;
#else
const vec4 inColor = vec4(1);
#endif
#if (LIT_INPUTS & 2)
layout(location = 4) in vec4 inTangent;
#else
const vec4 inTangent = vec4(1, 0, 0, 1);
#endif
layout(location = 0) out vec4 outColor;

mat3 buildTBN(vec3 normal) {
    vec3 tangent = normalize(inTangent.xyz - normal * dot(normal, inTangent.xyz));
    return mat3(tangent, cross(normal, tangent) * inTangent.w, normal);
}

void main() {
    uint flags      = uint(u_material.roughnessAlphaWorkflow.z + 0.5);
    vec4 baseSample = texture(u_baseColor, inTexCoord) * u_material.baseColor;
    if ((flags & 2u) != 0u) baseSample *= inColor;
    if ((flags & 4u) != 0u) baseSample.a = 1.0;
    float alphaCutoff = u_material.roughnessAlphaWorkflow.y;
    if (alphaCutoff > 0.0 && baseSample.a < alphaCutoff) { discard; }

    vec3 emissive = texture(u_emissive, inTexCoord).rgb * u_material.emissiveAndMetallic.rgb;

    vec3 N  = normalize(inNormal);
    vec3 Ns = texture(u_normal, inTexCoord).rgb * 2.0 - 1.0;
    if ((flags & 1u) != 0u) N = normalize(buildTBN(N) * Ns);
    if (!gl_FrontFacing) N = -N;
    vec3  V     = normalize(u_camera.cameraPosition.xyz - inWorldPos);
    float NdotV = max(dot(N, V), 0.0);

    // Primary directional light determination
    vec3 L          = normalize(vec3(0.5, 0.8, 0.5));
    vec3 lightColor = vec3(1.0);
    if (u_scene.numLights > 0u) {
        for (uint i = 0u; i < u_scene.numLights && i < MAX_SCENE_LIGHTS; ++i) {
            if (uint(u_scene.lights[i].positionOrDir.w + 0.5) == SCENE_LIGHT_TYPE_DIRECTIONAL) {
                L          = normalize(-u_scene.lights[i].positionOrDir.xyz);
                lightColor = u_scene.lights[i].colorAndRange.rgb;
                break;
            }
        }
    }

    float ao        = texture(u_occlusion, inTexCoord).r;
    vec3  arm       = texture(u_metalRough, inTexCoord).rgb;
    float metallic  = clamp(arm.b * u_material.emissiveAndMetallic.a, 0.0, 1.0);
    float roughness = clamp(arm.g * u_material.roughnessAlphaWorkflow.x, 0.04, 1.0);

    // 1. Multi-band cartoon diffuse (Half-Lambert wrap + stepped shadows)
    float NdotL         = dot(N, L);
    float halfLambert   = clamp(0.5 * NdotL + 0.5, 0.0, 1.0);
    float lightingCoord = halfLambert * ao;

    float shadow1 = smoothstep(u_cel.shadowParams.x - u_cel.shadowParams.y, u_cel.shadowParams.x + u_cel.shadowParams.y, lightingCoord);
    float shadow2 = smoothstep(u_cel.shadowParams.z - u_cel.shadowParams.w, u_cel.shadowParams.z + u_cel.shadowParams.w, lightingCoord);

    vec3 shadowColor       = mix(u_cel.deepShadowTint.rgb, u_cel.shadowTint.rgb, shadow2);
    vec3 diffuseMultiplier = mix(shadowColor, vec3(1.0), shadow1);
    vec3 diffuse           = baseSample.rgb * diffuseMultiplier * lightColor;

    // 2. Stepped Anime Specular
    vec3  H          = normalize(L + V);
    float NdotH      = max(dot(N, H), 0.0);
    float specLobe   = pow(NdotH, max(u_cel.specularParams.y, 1.0));
    float specFactor = smoothstep(u_cel.specularParams.x - 0.02, u_cel.specularParams.x + 0.02, specLobe);
    vec3  specColor  = mix(vec3(1.0), baseSample.rgb, metallic);
    vec3  specular   = specFactor * u_cel.specularParams.z * (1.0 - roughness * 0.7) * specColor * lightColor * shadow1;

    // 3. Stylized Rim Light (Fresnel silhouette glow)
    float fresnel   = 1.0 - NdotV;
    float rimVal    = pow(clamp(fresnel, 0.0, 1.0), 3.0);
    float rimFactor = smoothstep(u_cel.rimParams.x - u_cel.rimParams.y, u_cel.rimParams.x + u_cel.rimParams.y, rimVal);
    rimFactor *= clamp(halfLambert + 0.3, 0.0, 1.0);
    vec3 rim = rimFactor * u_cel.rimParams.z * u_cel.rimTint.rgb;

    // 4. Stylized ambient contribution
    vec3 irradiance = texture(sscIrradianceMap, N).rgb * u_scene.environmentLuminanceScale;
    if (u_scene.environmentAmbientFloor > 0.0) { irradiance = max(irradiance, vec3(u_scene.environmentAmbientFloor)); }
    vec3 ambient = baseSample.rgb * min(irradiance * 0.15, vec3(0.35)) * ao;

    // 5. Final combine & exposure
    vec3 lit = diffuse + specular + rim + ambient + emissive;
    lit *= u_camera.exposure;
    outColor = vec4(lit / (lit + vec3(1.0)), baseSample.a);
}
