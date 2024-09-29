#pragma once

#include "jenjin/mesh.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Jenjin {
namespace Helpers {
GLFWwindow* CreateWindow(int width, int height, const char* title);
void CheckWindow(GLFWwindow* window);
void InitiateImGui(GLFWwindow* window);

Mesh CreateQuad(int width, int height);
}
}
