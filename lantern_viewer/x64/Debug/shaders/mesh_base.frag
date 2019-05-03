// Fragment shader
#version 150

out vec4 frag_color;
in vec3 v_color;
in vec2 v_texture;

uniform sampler2D textureMap;

void main() {
    frag_color = texture(textureMap, v_texture);
}
