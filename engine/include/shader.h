#ifndef JENJIN_SHADER_H
#define JENJIN_SHADER_H

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Jenjin {
class Shader {
public:
	unsigned int ID; // ID of the shader program object

	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	void use();

	void set(const char* name, float value);
	void set(const char* name, int value);
	void set(const char* name, bool value);
	void set(const char* name, GLfloat *value);
	void set(const char* name, glm::mat4 value);
	void set(const char* name, glm::vec3 value);
};
}

#endif
