// Fragment shader
#version 150

out vec4 frag_color;

in vec4 v_position;

uniform samplerCube u_cubemap;

void main()
{
	frag_color = vec4(texture(u_cubemap, v_position.xyz).rgb , 1.0);
}