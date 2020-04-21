#version 330 core

in vec3 v_position;
in vec3 v_normal;
in vec2 v_texcoord;

out vec4 outputColor;

uniform vec3 l_position;
uniform vec3 l_color;
uniform vec3 color;

void main()
{
	vec3  l_normal = normalize(v_normal);
	vec3  l_direction = normalize(l_position - v_position);
	float l_angle = max(dot(l_normal, l_direction), 0.0);
	
	vec3  diffuse = l_color * l_angle * color;
	
	outputColor = vec4(diffuse, 1.0);
}