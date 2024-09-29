#version 460 core

out vec3 FragColor;

in vec3 j_fragColor;
in vec2 j_fragTexCoord;

void main() {
	FragColor = j_fragColor;
}
