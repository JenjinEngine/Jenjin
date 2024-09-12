#version 460 core

out vec3 FragColor;

in vec2 TexCoord;

uniform bool u_use_texture;
uniform bool u_texture_and_color;
uniform sampler2D u_texture;

uniform vec3 u_color;

void main() {
	// if `u_texture` wasn't even set
		// vec4 base = texture(u_texture, TexCoord);
		// vec4 base = texture(u_texture, TexCoord) * (u_texture_and_color ? vec4(u_color, 1.0) : vec4(1.0));
		// FragColor = base.rgb;

	// FragColor = u_texture_and_color ? texture(u_texture, TexCoord).rgb * u_color : texture(u_texture, TexCoord).rgb;

	if (!u_use_texture) {
		FragColor = u_color;
		return;
	}

	// basic texture
	FragColor = texture(u_texture, TexCoord).rgb;
}
