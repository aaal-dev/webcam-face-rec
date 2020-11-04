#version 330 core

in vec2 v_texcoord;

uniform sampler2D texture;

out vec4 outputColor;

void main() {
	outputColor = texture2D(texture, v_texcoord);
}