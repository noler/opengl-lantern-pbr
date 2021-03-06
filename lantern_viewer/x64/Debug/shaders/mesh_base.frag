// Fragment shader
#version 150

out vec4 frag_color;

in LIGHTING {
	flat int v_use_L0;
	flat int v_use_ambient_IBL;
	flat float v_strength;
} lightning_setting;

// boolean switches & solid colors/values instead of maps
in MATERIAL {
	vec3 v_albedo_color;
	flat int v_use_albedo_map;

	float v_roughness_value;
	flat int v_use_roughness_map;

	flat int v_use_metallic_map;
	flat float v_metallic_value;

	flat int v_use_normal_map;
	flat float v_normal_map_influence;
} material;

in UV_SPACE {
	vec2 v_texture_coord;
} uv;

in WORLD_SPACE {
	vec3 v_light_position;
	vec3 v_light_color;
	vec3 v_frag_pos;
	vec3 v_camera_position;
} world_in;

struct WORLD {
	vec3 normal_dir;
	vec3 view_dir;
	vec3 reflection_dir;
	vec3 light_dir;
	vec3 halfway_dir;
} world;

struct TANGENT_SPACE {
	vec3 normal_dir;
} tangent;

in mat3 v_tangent2world;

uniform sampler2D u_albedo_tex;
uniform sampler2D u_ambient_occlusion_tex;
uniform sampler2D u_metallic_tex;
uniform sampler2D u_normal_tex;
uniform sampler2D u_opacity_tex;
uniform sampler2D u_roughness_tex;

uniform samplerCube u_skybox;

// Constants 
float PI = 3.14159265359;
float MAX_MIPMAP_LEVEL = 8.0;

vec3 fresnel_schlick(vec3 F0, float cos_theta, float gloss) {
	return F0 + (max(F0, vec3(gloss)) - F0) * pow(1.0 - cos_theta, 5.0); 
}

vec3 gamma_correction(vec3 color) {
	return pow(color, vec3(1.0/2.2));
}

float geometry_schlick(float dot_prod, float roughness) {
	float k = ((roughness + 1) * (roughness + 1))/8;
	return dot_prod / (dot_prod * k + k);
}

float geometry_smith(vec3 N, vec3 L, vec3 V, float roughness) {
	return geometry_schlick(max(dot(N, V), 0), roughness) * geometry_schlick(max(dot(N, L), 0), roughness);
}

float distribution(float roughness, float dot_prod) {
	float r4 = pow(roughness, 4.0);
	return r4 / (dot_prod * dot_prod * (r4-1) + 1);
}

void main() {
	tangent.normal_dir = texture(u_normal_tex, uv.v_texture_coord).rgb;
	if (material.v_use_normal_map == 0) {
		tangent.normal_dir = vec3(0.5, 0.5, 1.0);
	}
	
	tangent.normal_dir = normalize((0.5*(2.0 * tangent.normal_dir) - 1.0));
	world.normal_dir = normalize(v_tangent2world * tangent.normal_dir);
	world.view_dir = normalize(world_in.v_camera_position - world_in.v_frag_pos);
	world.light_dir = normalize(world_in.v_light_position - world_in.v_frag_pos);
	world.halfway_dir = normalize(world.view_dir + world.light_dir);

	vec3 world_normal = normalize(v_tangent2world * tangent.normal_dir);
	
	world.reflection_dir = reflect(-world.view_dir, world.normal_dir);

	vec3 albedo = texture(u_albedo_tex, uv.v_texture_coord).xyz;
	if (material.v_use_albedo_map == 0) {
		albedo = material.v_albedo_color;
	}

	vec3 ambient_occlusion = texture(u_ambient_occlusion_tex, uv.v_texture_coord).xyz;
	
	float metallic = texture(u_metallic_tex, uv.v_texture_coord).r;
	if (material.v_use_metallic_map == 0) {
		metallic = material.v_metallic_value;
	}

	float roughness = texture(u_roughness_tex, uv.v_texture_coord).r;
	if (material.v_use_roughness_map == 0) {
		roughness = material.v_roughness_value;
	}

	float gloss = 1.0 - roughness;
	 

	// F0 is specular color, is estimated by linear interpolation
	vec3 F0 = mix(vec3(0.04), albedo, metallic);
	
	vec3 k_s = fresnel_schlick(F0, max(0, dot(world.halfway_dir, world.light_dir)), gloss);
	vec3 k_d = (1.0 - k_s) * metallic;

	float D = distribution(roughness, max(0, dot(world.normal_dir, world.halfway_dir)));
	vec3 F = k_s;
	float G = geometry_smith(world.normal_dir, world.light_dir, world.view_dir, roughness);

	vec3 specular = (D * G * F) / (4 * max(0, dot(world.normal_dir, world.view_dir)) * max(0, dot(world.normal_dir, world.light_dir)) + 0.0001);

	float attenuation = 1 / pow(length(world_in.v_light_position - world_in.v_frag_pos), 2.0);
	vec3 radiance = lightning_setting.v_strength * world_in.v_light_color; // * attenuation;
	float NdotL = max(0, dot(world.normal_dir, world.light_dir));

	vec3 L0 = (k_d * (albedo/PI) + specular) * radiance * NdotL;

	// Ambient lighning calculations (part of IBL)
	float theta_x = radians(180);
	float theta_y = radians(180);
	float theta_z = radians(-25);

	mat3 rot_x = mat3(vec3(1,0,0), vec3(0,cos(theta_x), sin(theta_x)), vec3(0, -sin(theta_x), cos(theta_x)));
	mat3 rot_y = mat3(vec3(cos(theta_y),0, -sin(theta_y)), vec3(0,1,0), vec3(sin(theta_y), 0, cos(theta_y)));
	mat3 rot_z = mat3(vec3(cos(theta_z),sin(theta_z), 0), vec3(-sin(theta_z),cos(theta_z),0), vec3(0,0,1));

	vec3 irradiance = textureLod(u_skybox, rot_z*rot_y*world.reflection_dir, gloss*MAX_MIPMAP_LEVEL).rgb;
	k_s = fresnel_schlick(F0, max(0, dot(world.normal_dir, world.view_dir)), gloss);
	k_d = 1.0 - k_s;
	vec3 diffuse = irradiance * albedo;

	vec3 ambient_IBL = k_d * diffuse * ambient_occlusion;

	frag_color = vec4(gamma_correction(lightning_setting.v_use_L0 * L0 + lightning_setting.v_use_ambient_IBL * ambient_IBL), 1.0);
}
