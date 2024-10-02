#version 460 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;

out vec3 j_fragColor;
out vec2 j_fragTexCoord;

uniform mat4 u_viewProjection;
uniform mat4 u_model;

void main() {
	gl_Position = u_viewProjection * u_model * vec4(position, 1.0);

	j_fragTexCoord = texCoord;
}
