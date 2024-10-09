#pragma once

#include <string>

class Texture {
public:
  unsigned int ID; // ID of the shader program object

  Texture(const char *imagePath, bool alpha = false);
  ~Texture();

  void bind(int id = 0);

private:
  std::string imagePath;
};
