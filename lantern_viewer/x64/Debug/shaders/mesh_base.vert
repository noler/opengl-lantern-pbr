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
// material colors/values uniforms 
uniform vec3 u_albedo_color;
uniform int u_use_albedo_map;
uniform float u_roughness_value;
uniform int u_use_roughness_map;

// material colors/values out
out vec3 v_albedo_color;
out float v_roughness_value;

// boolean switches
flat out int v_use_albedo_map;
flat out int v_use_roughness_map;



void main() 
{
	v_use_albedo_map = u_use_albedo_map;
	v_albedo_color = u_albedo_color;
	v_use_roughness_map = u_use_roughness_map;
	v_roughness_value = u_roughness_value;

	v_normal = a_normal;

	vec3 eye_position = normalize(mat3(u_mv) * a_normal);
	v_V = -1 * normalize(eye_position);
	v_R = reflect(-v_V, v_N);


	v_texture = a_texture;
    gl_Position = u_mvp * a_position;
}