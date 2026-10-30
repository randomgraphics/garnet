// Kernel-private stable packing for bindless unlit materials.
struct UnlitMaterialData {
    vec4  color;
    vec4  emissive;
    vec4  unused;
    uvec4 textureIndices;
};
layout(set = 0, binding = 0, std430) readonly buffer UnlitMaterialBuffer { UnlitMaterialData materials[]; }
materialHeap;
