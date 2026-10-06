// Kernel-private stable packing for bindless Cel materials.
struct CelMaterialData {
    vec4  baseColor;
    vec4  emissive;
    vec4  shadowParams;   // x = shadowThreshold, y = shadowFeather, z = deepShadowThreshold, w = deepShadowFeather
    vec4  shadowTint;     // rgb = shadowTint, a = unused
    vec4  deepShadowTint; // rgb = deepShadowTint, a = unused
    vec4  specularParams; // x = specularThreshold, y = specularShininess, z = specularIntensity, w = unused
    vec4  rimParams;      // x = rimThreshold, y = rimFeather, z = rimIntensity, w = unused
    vec4  rimTint;        // rgb = rimTint, a = unused
    vec4  outlineParams;  // x = outlineWidth, yzw = unused
    vec4  outlineColor;   // rgba = outlineColor
    uvec4 textureIndices; // x = colorIndex, y = normalIndex, z = samplerIndex, w = flags (bit 0: colorMap, bit 1: normalMap)
};
layout(set = 0, binding = 0, std430) readonly buffer CelMaterialBuffer {
    CelMaterialData materials[];
}
celMaterialHeap;
