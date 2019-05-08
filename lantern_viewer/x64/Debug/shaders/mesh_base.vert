// Vertex shader
#version 150
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texture;

out vec3 v_normal;
out vec3 v_N;		// Normal in view space
out vec3 v_V;
out vec3 v_R;

out vec2 v_texture;

uniform mat4 u_mv;
uniform mat4 u_mvp;

void main() 
{
	v_normal = a_normal;

	vec3 eye_position = normalize(mat3(u_mv) * a_normal);
	v_V = -1 * normalize(eye_position);
	v_R = reflect(-v_V, v_N);


	v_texture = a_texture;
    gl_Position = u_mvp * a_position;
}