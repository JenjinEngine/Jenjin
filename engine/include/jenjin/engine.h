#pragma once

#include "jenjin/scene.h"
#include "jenjin/target.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Jenjin {
class Engine {
public:
	Engine(GLFWwindow* window);

	void AddScene(std::shared_ptr<Scene> scene, bool defaultScene = false);

	void Render(Target* target); // Render directly to the target

	GLFWwindow* glfwContext;

private:
	std::vector<std::shared_ptr<Scene>> scenes = {};
	Scene* currentScene = nullptr;
};
}
