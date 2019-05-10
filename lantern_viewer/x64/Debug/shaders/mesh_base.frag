// Fragment shader
#version 150

out vec4 frag_color;
in vec2 v_texture_coord;

in vec3 v_N;		// Normal in world (model) space
in vec3 v_world_pos;
in vec3 v_camera_position;

// boolean switches
flat in int v_use_albedo_map;
flat in int v_use_roughness_map;

// solid colors/values instead of maps
in vec3 v_albedo_color;
in float v_ambient_occlusion_value;
in float v_metallic_value;
in float v_roughness_value;

in vec3 v_light_position;
in vec3 v_light_color;

uniform sampler2D u_albedo_tex;
uniform sampler2D u_ambient_occlusion_tex;
uniform sampler2D u_metallic_tex;
uniform sampler2D u_normal_tex;
uniform sampler2D u_opacity_tex;
uniform sampler2D u_roughness_tex;

uniform samplerCube u_cubemap;

in float v_test_value;
float PI = 3.14159265359;

void main() {
	vec3 N = v_N;
	vec3 V = normalize(v_camera_position - v_world_pos);
	vec3 R = reflect(-V, N);
	// vec3 L = ;


	vec3 albedo = v_albedo_color;
	if (v_use_albedo_map == 1) {
		albedo = texture(u_albedo_tex, v_texture_coord).rgb;
	}

	vec3 ambient_occlusion = texture(u_ambient_occlusion_tex, v_texture_coord).rgb;
	float metallic = texture(u_metallic_tex, v_texture_coord).r;
	vec3 normal = texture(u_normal_tex, v_texture_coord).rgb;

	float roughness = v_roughness_value;
	if (v_use_roughness_map == 1) {
		roughness = texture(u_roughness_tex, v_texture_coord).r;
	}
	
	
	/* --- Diffuseness --- */
	vec3 diffuse = albedo * ambient_occlusion / PI;
	
	/* --- Specularness --- */
	float mipmap_level = (1-roughness)*8;
	// float mipmap_level = 2.0;
	vec3 specular = textureLod(u_cubemap, R, mipmap_level).rgb;
	
    // frag_color = vec4(metallic*specular + (1-metallic) * diffuse, 1.0);
	frag_color = vec4(specular, 1.0);
}
