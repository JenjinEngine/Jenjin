#pragma once

#include <functional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

namespace JenjinEditor {
class Window {
public:
	Window(int width, int height, const char* title);

	void render(std::function<void(GLFWwindow*)> renderCallback) {
		m_renderCallback = renderCallback;
	}

	void launch();

	GLFWwindow* context = nullptr;

private:
	std::function<void(GLFWwindow*)> m_renderCallback;
};
}
