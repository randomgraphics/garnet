// Private vertex-stage ABI; draw-uniform fragment inputs are passed as varyings.
layout(push_constant, std430) uniform UnlitParameters {
    mat4 world;
    vec4 color;
    vec4 emissiveAndCutoff;
}
parameters;
