// Kernel-private stable packing for bindless Lambertian materials.
struct LambertianMaterialData {
    vec4  color;
    vec4  emissive;
    vec4  diffuseParameters;
    uvec4 textureIndices;
};
layout(set = 0, binding = 0, std430) readonly buffer LambertianMaterialBuffer { LambertianMaterialData materials[]; }
materialHeap;
