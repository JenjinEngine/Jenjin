#pragma once

#include <glad/glad.h>

namespace Jenjin {
class Framebuffer {
public:
  Framebuffer();
  ~Framebuffer();

  void Bind();
  void Unbind();

  void Resize(int width, int height);

  GLuint texture;

private:
  GLuint fbo;
  GLuint rbo;
};
} // namespace Jenjin
