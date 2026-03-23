#version 450
#extension GL_GOOGLE_include_directive : require

#include "environment-lighting-common.h"

layout(location = 0) in vec3 v_dir;
layout(location = 0) out vec4 o_color;

void main() {
    o_color = vec4(gn_tonemap(gn_sampleSkyboxRadiance(v_dir)), 1.0);
}
