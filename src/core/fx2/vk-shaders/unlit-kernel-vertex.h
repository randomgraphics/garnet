#include "camera-ubo.h"
#include "unlit-kernel-common.h"
layout(location = 0) in vec3 position;
#if USE_TEXTURE
layout(location = 2) in vec2 texcoord;
#endif
#if USE_COLOR
layout(location = 4) in vec4 color;
#endif
layout(location = 0) out vec2 uv;
layout(location = 1) out vec4 tint;
// gpu2 currently emits stage-specific push updates; keep this range vertex-only.
layout(location = 2) flat out vec4 emissiveAndCutoff;
void main() {
    gl_Position       = u_camera.projViewMatrix * parameters.world * vec4(position, 1);
    uv                = vec2(0);
    tint              = parameters.color;
    emissiveAndCutoff = parameters.emissiveAndCutoff;
#if USE_TEXTURE
    uv = texcoord;
#endif
#if USE_COLOR
    tint *= color;
#endif
}
