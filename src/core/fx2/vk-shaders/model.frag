#version 450
#extension GL_GOOGLE_include_directive : require

#include "scene-ubo.h"
#include "camera-ubo.h"
#include "model-material-ubo.h"

layout(set = 1, binding = 0) uniform sampler2D u_baseColor;
layout(set = 1, binding = 1) uniform sampler2D u_normal;
layout(set = 1, binding = 2) uniform sampler2D u_emissive;
layout(set = 1, binding = 3) uniform sampler2D u_occlusion;
layout(set = 1, binding = 4) uniform sampler2D u_metalRough;

layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec4 inColor;
layout(location = 4) in vec4 inTangent;
layout(location = 0) out vec4 outColor;

const float PI = 3.14159265359;

vec3 fresnelSchlickR(float cosTheta, vec3 f0, float roughness) {
    return f0 + (max(vec3(1.0 - roughness), f0) - f0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

mat3 buildTBN(vec3 normal) {
    vec3 tangent = normalize(inTangent.xyz - normal * dot(normal, inTangent.xyz));
    return mat3(tangent, cross(normal, tangent) * inTangent.w, normal);
}

void main() {
    vec4  baseSample = texture(u_baseColor, inTexCoord) * u_material.baseColor * inColor;
    vec3  emissive   = texture(u_emissive, inTexCoord).rgb * u_material.emissiveAndMetallic.rgb;
    vec3  arm        = texture(u_metalRough, inTexCoord).rgb;
    float metallic   = clamp(arm.b * u_material.emissiveAndMetallic.a, 0.0, 1.0);
    float roughness  = clamp(arm.g * u_material.roughnessAlphaWorkflow.x, 0.04, 1.0);
    float ao         = texture(u_occlusion, inTexCoord).r;
    uint  workflow   = uint(u_material.roughnessAlphaWorkflow.z + 0.5);
    if (workflow == 3u) {
        outColor = vec4(baseSample.rgb + emissive, baseSample.a);
        return;
    }

    vec3 N           = normalize(inNormal);
    vec3 Ns          = texture(u_normal, inTexCoord).rgb * 2.0 - 1.0;
    N                = normalize(buildTBN(N) * Ns);
    vec3  V          = normalize(u_camera.cameraPosition.xyz - inWorldPos);
    float nv         = max(dot(N, V), 0.0);
    vec3  f0         = mix(vec3(0.04), baseSample.rgb, metallic);
    vec3  fr         = fresnelSchlickR(nv, f0, roughness);
    vec3  kd         = (vec3(1.0) - fr) * (1.0 - metallic);
    vec3  irradiance = texture(sscIrradianceMap, N).rgb * u_scene.environmentRadianceScale;
    vec3  reflected  = reflect(-V, N);
    vec3  radiance   = textureLod(sscPrefilteredEnvMap, reflected, roughness * 4.0).rgb * u_scene.environmentRadianceScale;
    vec2  brdf       = texture(sscBrdfLut, vec2(nv, roughness)).rg;
    vec3  lit        = kd * baseSample.rgb * irradiance * ao + radiance * (fr * brdf.x + brdf.y) * ao + emissive;
    outColor         = vec4(lit / (lit + vec3(1.0)), baseSample.a);
}
