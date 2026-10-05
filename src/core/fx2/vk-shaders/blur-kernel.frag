#version 450
#extension GL_GOOGLE_include_directive : require
#define BLUR
#include "image-filter.h"
layout(location = 0) out vec4 outputColor;
void main() { outputColor = filterPixel(ivec2(gl_FragCoord.xy)); }
