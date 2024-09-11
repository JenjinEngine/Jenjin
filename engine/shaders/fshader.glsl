#version 460 core

out vec3 FragColor;

uniform vec3 u_color;

void main() {
	// FragColor = vec4(color, 1.0);
	FragColor = u_color;
}
