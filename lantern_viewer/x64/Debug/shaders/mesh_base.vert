// Vertex shader
#version 150
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texture;

out vec3 v_color;
out vec2 v_texture;

uniform mat4 u_mvp;

void main() 
{
	v_color = a_normal;
	v_texture = a_texture;
    gl_Position = u_mvp * a_position;
}