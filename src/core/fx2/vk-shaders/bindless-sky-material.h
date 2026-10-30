// Kernel-private stable packing for bindless sky materials.
struct SkyMaterialData {
    vec4  factors;
    uvec4 textureIndices;
    uvec4 samplerIndices;
    uvec4 unused;
};
layout(set = 0, binding = 0, std430) readonly buffer SkyMaterialBuffer { SkyMaterialData materials[]; }
skyMaterialHeap;
