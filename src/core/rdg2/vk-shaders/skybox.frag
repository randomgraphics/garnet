#version 450
#extension GL_GOOGLE_include_directive : require

#include "scene-ubo.h"

layout(location = 0) in vec3 v_dir;
layout(location = 0) out vec4 o_color;

vec3 gn_tonemap(vec3 radiance) {
    const float exposure = 0.002;
    vec3        c        = radiance * exposure;
    c                    = c / (c + vec3(1.0));
    return c;
}

void main() {
    vec3 raw = texture(sscSkyboxCubemap, v_dir).rgb * u_scene.environmentRadianceScale;
    o_color  = vec4(gn_tonemap(raw), 1.0);
}
