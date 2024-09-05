#include <spdlog/spdlog.h>

#include "window.h"
#include "GLFW/glfw3.h"
#include "state.h"

#define SHOW_ERROR(format) const char* error; glfwGetError(&error); spdlog::error(format, error);

using namespace Jenjin;

bool Window::NewWindow(int width, int height, const char* title) {
	if (!glfwInit()) {
		SHOW_ERROR("Failed to initialize GLFW: {}");
		return false;
	}

	// Create a windowed mode window and its OpenGL context
	m_window = glfwCreateWindow(width, height, title, NULL, NULL);
	if (!m_window) {
		SHOW_ERROR("Failed to create window: {}");
		glfwTerminate();
		return false;
	}

	// Make the m_window's context current
	glfwMakeContextCurrent(m_window);
	glfwSwapInterval(0);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		SHOW_ERROR("Failed to initialize GLAD: {}");
	} else {
		spdlog::info("OpenGL version: {}.{}", GLVersion.major, GLVersion.minor);
	}

	return true;
}

void Window::setResizeCallback(GLFWframebuffersizefun callback) {
	glfwSetFramebufferSizeCallback(m_window, callback);
}

void Window::setMouseCallback(GLFWcursorposfun callback) {
	glfwSetCursorPosCallback(m_window, callback);
}

void Window::setKeyCallback(GLFWkeyfun callback) {
	glfwSetKeyCallback(m_window, callback);
}
