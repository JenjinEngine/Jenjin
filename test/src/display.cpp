#include "display.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <spdlog/spdlog.h>

void Display::PreRender() {
	spdlog::trace("Display::PreRender");

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void Display::Render() {
	spdlog::trace("Display::Render");

	ImGui::Begin("Test UI");

	ImGui::Text("Hello, world!");

	ImGui::End();
}

void Display::PostRender() {
	spdlog::trace("Display::PostRender");

	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

glm::vec2 Display::GetSize() {
	spdlog::trace("Display::GetSize");

	int w,h; glfwGetWindowSize(glfwGetCurrentContext(), &w, &h);
	return glm::vec2(w, h);
}

void Display::Resize(glm::vec2 size) {
  glfwSetWindowSize(glfwGetCurrentContext(), size.x, size.y);
	glViewport(0, 0, size.x, size.y);
}

glm::vec2 Display::GetMousePosition() {
	double x,y; glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
	return glm::vec2(x, y);
}
