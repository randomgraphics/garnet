#version 450

layout(set = 0, binding = 2) uniform samplerCube u_skyboxCubemap;

layout(location = 0) in  vec3 v_dir;
layout(location = 0) out vec4 o_color;

void main() {
    o_color = texture(u_skyboxCubemap, v_dir);
}
