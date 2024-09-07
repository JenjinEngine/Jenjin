#version 330 core

out vec4 FragColor;

uniform vec3 go_color;

void main() {
	FragColor = vec4(go_color, 1.0f);
}
