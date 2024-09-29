#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 color;
layout(location = 2) in vec2 texCoord;

out vec3 j_fragColor;
out vec2 j_fragTexCoord;

void main() {
	gl_Position = vec4(position, 2.0);

	j_fragColor = color;
	j_fragTexCoord = texCoord;
}
