#include <spdlog/spdlog.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "jenjin/engine.h"

using namespace Jenjin;

const char* VERSION = "0.0.1";

Engine::Engine(GLFWwindow* window) {
	spdlog::set_level(spdlog::level::trace);
	spdlog::debug("Initializing Jenjin {}", VERSION);

	glfwSetErrorCallback([](int code, const char* error) {
		spdlog::error("GLFW Error: {} ({})", error, code);
	});

	// Intialize GLFW, logging any errors
	if (glfwGetKeyScancode(GLFW_KEY_ESCAPE) == GLFW_NOT_INITIALIZED) {
		spdlog::error("GLFW isn't initialized, Jenjin needs GLFW externally initialized");
		exit(EXIT_FAILURE);
	}

	// Check if the engine has a window
	if (!window) {
		spdlog::error("There isn't a glfwContext to render to, Jenjin needs a window to render to");
		exit(EXIT_FAILURE);
	}

	glfwContext = window;

	// Make the glfwContext's context current
	glfwMakeContextCurrent(glfwContext);

	// Load OpenGL functions using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		spdlog::error("Failed to initialize GLAD");
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	spdlog::debug("OpenGL Version: {}", (const char*)glGetString(GL_VERSION));
	spdlog::debug("GLSL Version: {}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	spdlog::debug("Vendor: {}", (const char*)glGetString(GL_VENDOR));

	// Set escape key to close glfwContext
	glfwSetKeyCallback(glfwContext, [](GLFWwindow* glfwContext, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(glfwContext, GLFW_TRUE);
		}
	});
}

void Engine::Render(Target* target) {
	spdlog::trace("Engine::Render({})", (void*)target);

	target->PreRender();

	// render stuff
	target->Render();

	target->PostRender();
}
