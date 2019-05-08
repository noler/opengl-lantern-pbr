// Fragment shader
#version 150

out vec4 frag_color;
in vec2 v_texture;

in vec3 v_normal;
in vec3 v_N;		// Normal in view space
in vec3 v_V;
in vec3 v_R;

uniform sampler2D albedoTex;
uniform sampler2D metallicTex;
uniform sampler2D ambientOcclusionTex;
uniform sampler2D normalTex;
uniform sampler2D OpacityTex;
uniform sampler2D RoughnessTex;

uniform samplerCube u_cubemap;

void main() {
	vec3 ambient_occlusion = texture(ambientOcclusionTex, v_texture).xyz;
	vec3 albedo = texture(albedoTex, v_texture).xyz;
	
	vec3 diffuse = albedo * ambient_occlusion / 3.1415;
	
	vec3 specular = texture(u_cubemap, v_R).rgb;

    frag_color = vec4(specular, 1.0);
}
