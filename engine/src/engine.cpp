#include <spdlog/spdlog.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "engine.h"

using namespace Jenjin;

const char* VERSION = "0.0.1";

Engine::Engine() {
// Enable debug logging if in debug mode
#ifndef NDEBUG
	spdlog::set_level(spdlog::level::trace);
#endif

	spdlog::debug("Initializing Jenjin {}", VERSION);

	// Intialize GLFW, logging any errors
	if (!glfwInit()) {
		const char* error;
		int code = glfwGetError(&error);
		spdlog::error("Failed to initialize GLFW: {} ({})", error, code);
	}

	// Set OpenGL version to 4.6 Core Profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Enable forward compatibility on macOS
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a hidden window to initialize OpenGL context and load GLAD
	// This is to easily construct types throughout engine initialization
	// that require an OpenGL context
	window = glfwCreateWindow(1, 1, "Jenjin", NULL, NULL);

	// Check if window creation failed
	if (!window) {
		const char* error;
		int code = glfwGetError(&error);
		spdlog::error("Failed to create GLFW window: {} ({})", error, code);
		glfwTerminate();

		throw std::runtime_error("Failed to create GLFW window");
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Load OpenGL functions using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		spdlog::error("Failed to initialize GLAD");
		glfwTerminate();

		throw std::runtime_error("Failed to initialize GLAD");
	}

	spdlog::debug("OpenGL Version: {}", (const char*)glGetString(GL_VERSION));
	spdlog::debug("GLSL Version: {}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	spdlog::debug("Vendor: {}", (const char*)glGetString(GL_VENDOR));

	// Set escape key to close window
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		// key to toggle wireframe
		static bool wireframe = false;
		if (key == GLFW_KEY_T && action == GLFW_PRESS) {
			wireframe = !wireframe;
			glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
		}
	});
}

void Engine::add_scene(Scene* scene, bool active) {
	m_scenes.push_back(scene);

	if (active)
		m_active_scene = scene;
}

void Engine::activate_scene(unsigned int index) {
	if (index < 0 || index >= m_scenes.size()) {
		spdlog::error("Invalid scene index: {}", index);
		return;
	}

	m_active_scene = m_scenes[index];
}

void Engine::launch(int width, int height, const char* title) {
	// Build all the scenes
	for (auto& scene : m_scenes)
	scene->build();

	// Setup the window to be the desired size and title
	// instead of the hidden window
	glfwSetWindowSize(window, width, height);
	glfwSetWindowTitle(window, title);

	// Tell OpenGL to draw to the entire window
	glViewport(0, 0, width, height);

	// Main loop
	glClearColor(0.07f, 0.13f, 0.17f, 1.0);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		// Render the active scene
		if (m_active_scene) {
			m_active_scene->render();
		} else {
			spdlog::warn("No active scene");
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	spdlog::debug("Deinitializing Jenjin {}", VERSION);

	for (auto& scene : m_scenes)
	delete scene;

	spdlog::debug("Terminating GLFW");
	glfwTerminate();

	/* spdlog::debug("Deinitializing Jenjin {}", VERSION); */

	/* spdlog::debug("Terminating GLFW"); */
	/* glfwTerminate(); */
}
