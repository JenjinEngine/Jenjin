#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
	unsigned int ID; // ID of the shader program object

	Texture(const char* imagePath, bool alpha = false);
	~Texture();

	void bind(int id = 0);
};

#endif
