#pragma once

#ifdef __cplusplus
    #include <glm/vec4.hpp>
    #define vec4 glm::vec4
namespace GN::fx2::shader {
#endif

struct ModelMaterialUBO {
    vec4 baseColor;
    vec4 emissiveAndMetallic;
    vec4 roughnessAlphaWorkflow;
};

#ifdef __cplusplus
    #undef vec4
static_assert(sizeof(ModelMaterialUBO) == 48);
} // namespace GN::fx2::shader
#else
layout(std140, set = 1, binding = 5) uniform ModelMaterialBlock {
    vec4 baseColor;
    vec4 emissiveAndMetallic;
    vec4 roughnessAlphaWorkflow;
}
u_material;
#endif
