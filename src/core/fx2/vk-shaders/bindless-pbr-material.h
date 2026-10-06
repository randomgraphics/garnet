struct PbrMaterialData {
    vec4  baseColor;
    vec4  emissive;
    vec4  factors;        // metallic, roughness, normal scale, occlusion strength
    uvec4 textureIndices; // base color, normal, emissive, occlusion
    uvec4 extraIndices;   // metal/roughness, sampler, map flags, opaque
};
layout(set = 0, binding = 0, std430) readonly buffer PbrMaterialBuffer { PbrMaterialData materials[]; }
pbrMaterialHeap;
