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
flat in int v_use_L0;
flat in int v_use_ambient_IBL;

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
	vec3 N = v_N;
	vec3 V = normalize(v_camera_position - v_world_pos);
	vec3 R = reflect(-V, N);

	vec3 albedo = texture(u_albedo_tex, v_texture_coord).xyz;
	vec3 ambient_occlusion = texture(u_ambient_occlusion_tex, v_texture_coord).xyz;
	float metallic = texture(u_metallic_tex, v_texture_coord).r;
	vec3 normal = texture(u_normal_tex, v_texture_coord).rgb;
	float roughness = texture(u_roughness_tex, v_texture_coord).r;
	float gloss = 1.0 - roughness;
	vec3 irradiance = textureLod(u_cubemap, R, MAX_MIPMAP_LEVEL).rgb; 

	// F0 is specular color, is estimated by linear interpolation
	vec3 F0 = mix(vec3(0.04), albedo, metallic);

	// Reflectance calcuations (not part of IBL)
	vec3 L = normalize(v_light_position - v_world_pos);
	vec3 H = normalize(V + L);
	
	vec3 k_s = fresnel_schlick(F0, max(0, dot(H, V)), gloss);
	vec3 k_d = (1.0 - k_s) * metallic;

	float D = distribution(roughness, max(0, dot(N, H)));
	vec3 F = k_s;
	float G = geometry_smith(N, L, V, roughness);

	vec3 specular = (D * G * F) / (4 * max(0, dot(N, V)) * max(0, dot(N, L)) + 0.0001);

	float attenuation = 1 / pow(length(v_light_position - v_world_pos), 2.0);
	vec3 radiance = v_light_color; // * attenuation;
	float NdotL = max(0, dot(N, L));

	vec3 L0 = (k_d * (albedo/PI) + specular) * radiance * NdotL;

	// Ambient lighning calculations (part of IBL)
	k_s = fresnel_schlick(F0, max(0, dot(N, V)), gloss);
	k_d = 1.0 - k_s;
	vec3 diffuse = irradiance * albedo;

	vec3 ambient_IBL = k_d * diffuse * ambient_occlusion;

	frag_color = vec4(gamma_correction(v_use_L0 * L0 + v_use_ambient_IBL * ambient_IBL), 1.0);
}
