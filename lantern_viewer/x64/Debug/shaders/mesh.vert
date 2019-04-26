// Vertex shader
#version 150
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec4 a_position;

// uniform float u_time;
uniform mat4 u_trans;

void main() {
    gl_Position = a_position;

}