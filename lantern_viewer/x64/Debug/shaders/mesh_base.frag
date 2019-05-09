// Fragment shader
#version 150

out vec4 frag_color;
in vec2 v_texture;

in vec3 v_normal;
in vec3 v_N;		// Normal in view space
in vec3 v_V;
in vec3 v_R;

// boolean switches
flat in int v_use_albedo_map;

// solid colors/values instead of maps
in vec3 v_albedo_color;
in float v_ambient_occlusion_value;
in float v_metallic_value;
in float v_roughness_value;

uniform sampler2D albedoTex;
uniform sampler2D ambientOcclusionTex;
uniform sampler2D metallicTex;
uniform sampler2D normalTex;
uniform sampler2D OpacityTex;
uniform sampler2D RoughnessTex;

uniform samplerCube u_cubemap;

in float v_test_value;

void main() {
	
	vec3 albedo;
	if (v_use_albedo_map == 1) {
		albedo = texture(albedoTex, v_texture).xyz;
	} else {
		albedo = v_albedo_color;
	}
	
	vec3 ambient_occlusion = texture(ambientOcclusionTex, v_texture).xyz;
	float metallic = texture(metallicTex, v_texture).r;
	vec3 normal = texture(normalTex, v_texture).rgb;
	float roughness = texture(RoughnessTex, v_texture).r;
	
	
	/* --- Diffuseness --- */
	vec3 diffuse = albedo * ambient_occlusion / 3.1415;
	

	/* --- Specularness --- */
	float mipmap_level = roughness*10;
	vec3 specular = textureLod(u_cubemap, v_R, mipmap_level).rgb;

	
	
    frag_color = vec4(metallic*specular + (1-metallic) * diffuse, 1.0);
}
