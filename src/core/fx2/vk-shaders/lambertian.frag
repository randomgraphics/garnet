#version 450
#extension GL_GOOGLE_include_directive : require
#include "scene-ubo.h"
#include "camera-ubo.h"
#include "model-material-ubo.h"
layout(set = 1, binding = 0) uniform sampler2D u_baseColor;
layout(set = 1, binding = 1) uniform sampler2D u_normal;
layout(location = 0) in vec3 inWorldPos;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in vec2 inTexCoord;
layout(location = 3) in vec4 inColor;
layout(location = 4) in vec4 inTangent;
layout(location = 0) out vec4 outColor;
void main() {
    uint flags = uint(u_material.roughnessAlphaWorkflow.z + 0.5);
    vec4 c     = texture(u_baseColor, inTexCoord) * u_material.baseColor;
    if ((flags & 2u) != 0u) c *= inColor;
    if (c.a < u_material.roughnessAlphaWorkflow.y) discard;
    if ((flags & 4u) != 0u) c.a = 1.0;
    vec3 n = normalize(inNormal);
    if ((flags & 1u) != 0u) {
        vec3 t = normalize(inTangent.xyz - n * dot(n, inTangent.xyz));
        n      = normalize(mat3(t, cross(n, t) * inTangent.w, n) * (texture(u_normal, inTexCoord).xyz * 2.0 - 1.0));
    }
    if (!gl_FrontFacing) n = -n;
    vec3 irradiance = max(texture(sscIrradianceMap, n).rgb * u_scene.environmentLuminanceScale, vec3(u_scene.environmentAmbientFloor));
    vec3 diffuse    = irradiance * u_material.roughnessAlphaWorkflow.w;
    for (uint i = 0u; i < min(u_scene.numLights, uint(MAX_SCENE_LIGHTS)); ++i) {
        DirectLightData light           = u_scene.lights[i];
        uint            type            = uint(light.positionOrDir.w + 0.5);
        vec3            delta           = light.positionOrDir.xyz - inWorldPos;
        float           distanceSquared = max(dot(delta, delta), 0.0001);
        vec3            l               = type == SCENE_LIGHT_TYPE_DIRECTIONAL ? normalize(-light.positionOrDir.xyz) : delta * inversesqrt(distanceSquared);
        float           attenuation     = type == SCENE_LIGHT_TYPE_DIRECTIONAL ? 1.0 : 1.0 / distanceSquared;
        if (type != SCENE_LIGHT_TYPE_DIRECTIONAL && light.colorAndRange.w > 0.0)
            attenuation *= pow(clamp(1.0 - sqrt(distanceSquared) / light.colorAndRange.w, 0.0, 1.0), 2.0);
        // Scene light data currently has no spot direction; follow its point-light interpretation.
        diffuse += light.colorAndRange.rgb * attenuation * max(dot(n, l), 0.0) / 3.14159265359;
    }
    vec3 lit = (c.rgb * diffuse + u_material.emissiveAndMetallic.rgb) * u_camera.exposure;
    outColor = vec4(lit / (lit + vec3(1)), c.a);
}
