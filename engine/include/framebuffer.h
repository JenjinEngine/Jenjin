#pragma once

#include <glad/glad.h>

namespace Jenjin {
class Framebuffer {
public:
	Framebuffer();
	~Framebuffer();

	void bind();
	void unbind();

	void resize(int width, int height);

	GLuint texture;

private:
	GLuint fbo;
	GLuint rbo;
};
}
