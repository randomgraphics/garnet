// Vertex-only push range; backend mapping is private to these kernels.
layout(push_constant, std430) uniform SimpleDraw {
    mat4  worldFromObject;
    uvec4 materialIndex;
}
draw;
