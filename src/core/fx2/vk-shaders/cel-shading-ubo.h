#pragma once

#ifdef __cplusplus
    #include <glm/vec4.hpp>
    #define vec4 glm::vec4
namespace GN::fx2::shader {
#endif

struct CelShadingUBO {
    vec4 shadowParams;   ///< x = shadowThreshold, y = shadowFeather, z = deepShadowThreshold, w = deepShadowFeather
    vec4 shadowTint;     ///< rgb = shadowTint, a = unused
    vec4 deepShadowTint; ///< rgb = deepShadowTint, a = unused
    vec4 specularParams; ///< x = specularThreshold, y = specularShininess, z = specularIntensity, w = unused
    vec4 rimParams;      ///< x = rimThreshold, y = rimFeather, z = rimIntensity, w = unused
    vec4 rimTint;        ///< rgb = rimTint, a = unused
    vec4 outlineParams;  ///< x = outlineWidth, yzw = unused
    vec4 outlineColor;   ///< rgba = outlineColor
};

#ifdef __cplusplus
    #undef vec4
static_assert(sizeof(CelShadingUBO) == 128);
} // namespace GN::fx2::shader
#else
layout(std140, set = 1, binding = 6) uniform CelShadingBlock {
    vec4 shadowParams;
    vec4 shadowTint;
    vec4 deepShadowTint;
    vec4 specularParams;
    vec4 rimParams;
    vec4 rimTint;
    vec4 outlineParams;
    vec4 outlineColor;
}
u_cel;
#endif
