#include <glad/glad.h>
#include <GL/gl.h>
#include <spdlog/spdlog.h>

#include "engine.h"
#include "GLFW/glfw3.h"
#include "state.h"

const char* ENGINE_VERSION = "0.0.1";

using namespace Jenjin;

Engine::Engine() {
	this->m_active_scene = nullptr;
	this->check_version();

	this->m_window = Window();

	#ifndef NDEBUG
		spdlog::set_level(spdlog::level::debug);
	#endif
}

void Engine::check_version(void) {
	spdlog::info("Jenjin engine version: {}", ENGINE_VERSION);
}

void Engine::add_scene(const Scene& scene) {
	m_scenes.emplace_back(scene);
}

bool Engine::activate_scene(int index) {
	if (index < m_scenes.size()) {
		m_active_scene = &m_scenes[index];
	} else {
		spdlog::error("Scene index out of bounds");
		return false;
	}

	return true;
}

void Engine::launch(int width, int height, const char* title) {
	spdlog::info("Launching Jenjin engine");

	if (!m_window.NewWindow(width, height, title)) {
		spdlog::error("Failed to create window");
		return;
	}

	for (Scene& scene : m_scenes)
		scene.build();

	GLFWwindow* window = this->m_window.getWindow();

	// handle window resize
	m_window.setResizeCallback([](GLFWwindow* window, int width, int height) {
		glViewport(0, 0, width, height);
	});

	// handle escape key (close window on press)
	m_window.setKeyCallback([](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
	});

	while (!glfwWindowShouldClose(window)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		float currentFrame = glfwGetTime();
		JenjinState.deltaTime = currentFrame - m_lastFrame;

		if (m_render_callback != nullptr)
			m_render_callback(this, window);

		if (m_active_scene != nullptr) {
			m_active_scene->render();
		} else {
			spdlog::warn("No active scene");
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
		m_lastFrame = currentFrame;
	}

	spdlog::info("Terminating GLFW");
	glfwTerminate();
}

void Engine::set_resized_callback(GLFWwindowsizefun callback) {
	m_window.setResizeCallback(callback);
}

void Engine::set_key_callback(GLFWkeyfun callback) {
	m_window.setKeyCallback(callback);
}

void Engine::set_mouse_callback(GLFWcursorposfun callback) {
	m_window.setMouseCallback(callback);
}

void Engine::set_render_callback(std::function<void(Engine*, GLFWwindow*)> callback) {
	m_render_callback = callback;
}
