// Kernel-private stable packing; public MaterialParameters need not match these fields.
struct SimpleMaterialData {
    vec4  color;
    vec4  emissiveAndCutoff;
    vec4  diffuseAndOpaque;
    uvec4 textureIndices;
};
layout(set = 0, binding = 0, std430) readonly buffer SimpleMaterialBuffer { SimpleMaterialData materials[]; }
materialHeap;
