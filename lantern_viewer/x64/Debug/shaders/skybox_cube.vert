// Vertex shader
#version 150
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec4 a_position;

uniform mat4 u_mvp;
out vec4 v_position;

void main()
{
	v_position = a_position;
	gl_Position = u_mvp * a_position;
}