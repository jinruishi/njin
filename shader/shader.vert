#version 450

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec4 tangent;
layout (location = 3) in vec2 tex_coord;

layout (location = 0) out vec3 frag_color;


void main() {
    gl_Position = vec4(position, 1);
    frag_color = vec3(1.0, 0.0, 0.0);
}