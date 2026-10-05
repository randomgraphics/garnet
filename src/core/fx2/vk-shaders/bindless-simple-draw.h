// Vertex-only push range: gpu2 emits stage-specific push updates.
layout(push_constant, std430) uniform SimpleDraw {
    mat4  worldFromObject;
    vec4  color;
    vec4  emissiveAndCutoff;
    vec4  diffuseAndOpaque;
    uvec4 textureIndices;
}
draw;
