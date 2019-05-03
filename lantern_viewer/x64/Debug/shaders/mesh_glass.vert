// Vertex shader
#version 150
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec3 a_normal;

out vec3 v_color;

uniform mat4 u_mvp;

void main() 
{
	v_color = vec3(0.9,0.9,0.9);
    gl_Position = u_mvp * a_position;
}