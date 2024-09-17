#include "shader.h"

#include <cstdlib>
#include <filesystem>
#include <spdlog/spdlog.h>

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>

using namespace Jenjin;

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	spdlog::trace("Shader::Shader(\"{}\", \"{}\")", vertexPath, fragmentPath);

	// Code buffers
	std::string vertexCode;
	std::string fragmentCode;

	// File streams
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	// Enable exceptions for reading files
	vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

	try {
		// Open files into file streams
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);

		// Read file streams into string streams
		std::stringstream vShaderStream, fShaderStream;

		// Stream file contents into string streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		// Close file streams
		vShaderFile.close();
		fShaderFile.close();

		// Convert string streams to strings
		spdlog::debug("Reading vertex shader file: `{}`", vertexPath);
		vertexCode   = vShaderStream.str();
		spdlog::debug("Reading fragment shader file: `{}`", fragmentPath);
		fragmentCode = fShaderStream.str();
	} catch(std::ifstream::failure &e) {
		spdlog::error("Current working directory: {}", std::filesystem::current_path().string());
		spdlog::error("Failed to read shader files: `{}` `{}` for reason: `{}`", vertexPath, fragmentPath, e.what());
	}

	// C-style strings for OpenGL
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// Shader objects (references via ID)
	unsigned int vertex, fragment = 0;

	// Used to check for errors
	int success;
	char infoLog[512];

	// Vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	// Show any compilation errors
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		spdlog::error("Failed to compile vertex shader:\n{}", infoLog);
	};

	// Fragment shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	// Show any compilation errors
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if(!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		spdlog::error("Failed to compile fragment shader:\n{}", infoLog);
	};

	// Create a program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	// Show any linking errors
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if(!success) {
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		spdlog::error("Failed to link shaders: {}", infoLog);
	}

	// Shader objects are now copied into the program and can be deleted
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader() {
	spdlog::trace("Shader::~Shader()");

	glDeleteProgram(ID);
}

void Shader::use() {
	glUseProgram(ID);
}

void Shader::set(const char* name, float value) {
	glUniform1f(glGetUniformLocation(ID, name), value);
}

void Shader::set(const char* name, int value) {
	glUniform1i(glGetUniformLocation(ID, name), value);
}

void Shader::set(const char* name, bool value) {
	glUniform1i(glGetUniformLocation(ID, name), (int)value);
}

void Shader::set(const char* name, GLfloat *value) {
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, value);
}

void Shader::set(const char* name, glm::mat4 value) {
	glUniformMatrix4fv(glGetUniformLocation(ID, name), 1, GL_FALSE, glm::value_ptr(value));
}

void Shader::set(const char* name, glm::vec3 value) {
	glUniform3fv(glGetUniformLocation(ID, name), 1, glm::value_ptr(value));
}
