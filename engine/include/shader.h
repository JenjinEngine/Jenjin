#pragma once

#include <glad/glad.h>
#include <glm.hpp>

namespace Jenjin {
// Shader class, helps to load and manage shaders
class Shader {
public:
	unsigned int ID; // ID of the shader program object

	// RAII pattern
	Shader(const char* vertexPath, const char* fragmentPath);
	~Shader();

	// Apply the shader
	void use();

	// Utility uniform functions for setting values
	// without using many different functions
	void set(const char* name, float value);
	void set(const char* name, int value);
	void set(const char* name, bool value);
	void set(const char* name, GLfloat *value);
	void set(const char* name, glm::mat4 value);
	void set(const char* name, glm::vec3 value);
};
}
