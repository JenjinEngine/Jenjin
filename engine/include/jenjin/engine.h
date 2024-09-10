#pragma once

#include "jenjin/target.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Jenjin {
class Engine {
public:
	Engine(GLFWwindow* window);

	void Render(Target* target); // Render directly to the target

	GLFWwindow* glfwContext;
};
}
