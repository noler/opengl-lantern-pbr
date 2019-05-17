// Vertex shader
#version 150
#extension GL_ARB_explicit_attrib_location : require

layout(location = 0) in vec4 a_position;
layout(location = 1) in vec3 a_normal;
layout(location = 2) in vec2 a_texture_coord;
layout(location = 3) in vec3 a_tangent;
layout(location = 4) in vec3 a_bitangent;

out LIGHTING {
	flat int v_use_L0;
	flat int v_use_ambient_IBL;
} lightning_setting;

// material colors/values out
out MATERIAL {
	vec3 v_albedo_color;
	flat int v_use_albedo_map;

	float v_roughness_value;
	flat int v_use_roughness_map;
} material;

out UV_SPACE {
	vec2 v_texture_coord;
} uv;

out WORLD_SPACE {
	vec3 v_light_position;
	vec3 v_light_color;
	vec3 v_frag_pos;
	vec3 v_camera_position;

} world_out;

out mat3 v_tangent2world;
out mat3 v_world2tangent;


// light
uniform vec3 u_light_position;
uniform vec3 u_light_color;
uniform vec3 u_camera_position;

uniform mat4 u_m;
uniform mat4 u_mvp;
// material colors/values uniforms 
uniform vec3 u_albedo_color;
uniform int u_use_albedo_map;
uniform float u_roughness_value;
uniform int u_use_roughness_map;
uniform int u_use_L0;
uniform int u_use_ambient_IBL;

/*
 * PRE: local_tangent, local_bitangent, local_normal are in the meshs own local coordinate system, NOT worldspace.
 * PRE: _model matrix converts from mesh-local to worldspace.
 * POST: returns matrix that converts from tangentspace to worldspace.
*/
mat3 get_tangent_to_world_matrix(mat4 _model, vec3 local_tangent, vec3 local_bitangent, vec3 local_normal) {
	vec3 T = normalize(vec3(_model * vec4(local_tangent, 0.0)));
	vec3 B = normalize(vec3(_model * vec4(local_bitangent, 0.0)));
	vec3 N = normalize(vec3(_model * vec4(local_normal, 0.0)));
	return mat3(T, B, N);
}

out vec3 v_light_position;
out vec3 v_light_color;

out vec3 v_world_pos;
out vec3 v_camera_position;

// boolean switches
flat out int v_use_albedo_map;
flat out int v_use_roughness_map;

void main() 
{	
	v_tangent2world = get_tangent_to_world_matrix(u_m, a_tangent, a_bitangent, a_normal);

	world_out.v_frag_pos = mat3(u_m) * a_position.xyz;
	world_out.v_light_color = u_light_color;
	world_out.v_light_position = u_light_position;
	world_out.v_camera_position = u_camera_position;

	material.v_use_albedo_map = u_use_albedo_map;
	material.v_albedo_color = u_albedo_color;
	material.v_use_roughness_map = u_use_roughness_map;
	material.v_roughness_value = u_roughness_value;
	
	lightning_setting.v_use_L0 = u_use_L0;
	lightning_setting.v_use_ambient_IBL = u_use_ambient_IBL;

	uv.v_texture_coord = a_texture_coord;

    gl_Position = u_mvp * a_position;
}