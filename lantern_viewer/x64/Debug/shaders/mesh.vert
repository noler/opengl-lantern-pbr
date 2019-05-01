// Vertex shader
#version 150
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec4 a_position;

uniform mat4 u_mvp;

void main() 
{
    gl_Position = u_mvp * a_position;
}