#version 460 core

out vec3 FragColor;

in vec2 j_fragTexCoord;

uniform sampler2D u_texture;
uniform bool u_hasTexture;
uniform bool u_mixColor;

uniform vec3 u_color;

void main() {
	if (!u_hasTexture) {
		FragColor = u_color;
		return;
	}

	FragColor = texture(u_texture, j_fragTexCoord).rgb;

	if (u_mixColor) {
		FragColor *= u_color;
	}
}
