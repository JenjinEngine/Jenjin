#version 460 core

out vec3 FragColor;
// in vec2 j_fragTexCoord;

uniform vec3 u_color;

void main() {
	FragColor = u_color;
}
