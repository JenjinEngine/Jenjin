#ifndef JENJIN_FRAMEBUFFER_H
#define JENJIN_FRAMEBUFFER_H

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

#endif // JENJIN_FRAMEBUFFER_H
