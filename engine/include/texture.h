#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>

class Texture {
public:
	unsigned int ID; // ID of the shader program object

	Texture(const char* imagePath, bool alpha = false);
	~Texture();

	void bind(int id = 0);

private:
	std::string m_image_path;
};

#endif
