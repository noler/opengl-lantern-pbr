// Fragment shader
#version 150

out vec4 frag_color;

in vec4 v_position;

uniform samplerCube u_cubemap;

vec3 gamma_correction(vec3 color) {
	return pow(color, vec3(1.0/2.2));
}

void main()
{
	frag_color = vec4(gamma_correction(texture(u_cubemap, v_position.xyz).rgb) , 1.0);
}