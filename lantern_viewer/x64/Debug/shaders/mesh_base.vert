// Vertex shader
#version 150
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texture_coord;

out vec3 v_N;		// Normal in view space

out vec2 v_texture_coord;

// light
uniform vec3 u_light_position;
uniform vec3 u_light_color;
uniform vec3 u_camera_position;

uniform mat4 u_m;
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

out vec3 v_light_position;
out vec3 v_light_color;

out vec3 v_world_pos;
out vec3 v_camera_position;

// boolean switches
flat out int v_use_albedo_map;
flat out int v_use_roughness_map;

void main() 
{	
	v_world_pos = mat3(u_m) * a_position.xyz;
	v_N = mat3(u_m) * a_normal;

	v_use_albedo_map = u_use_albedo_map;
	v_albedo_color = u_albedo_color;
	v_use_roughness_map = u_use_roughness_map;
	v_roughness_value = u_roughness_value;

	
	

	v_light_color = u_light_color;
	v_light_position = vec3(10,10,0);

	v_texture_coord = a_texture_coord;
    gl_Position = u_mvp * a_position;
}