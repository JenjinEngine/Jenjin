#include "mydisplay.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <spdlog/spdlog.h>

void MyDisplay::PreRender() {
	spdlog::trace("MyDisplay::PreRender");

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void MyDisplay::Render() {
	spdlog::trace("MyDisplay::Render");

	ImGui::Begin("Test UI");

	ImGui::Text("Hello, world!");

	ImGui::End();
}

void MyDisplay::PostRender() {
	spdlog::trace("MyDisplay::PostRender");

	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

glm::vec2 MyDisplay::GetSize() {
	spdlog::trace("MyDisplay::GetSize");

	int w,h; glfwGetWindowSize(glfwGetCurrentContext(), &w, &h);
	return glm::vec2(w, h);
}

void MyDisplay::Resize(glm::vec2 size) {
	glfwSetWindowSize(glfwGetCurrentContext(), size.x, size.y);
	glViewport(0, 0, size.x, size.y);
}

glm::vec2 MyDisplay::GetMousePosition() {
	double x,y; glfwGetCursorPos(glfwGetCurrentContext(), &x, &y);
	return glm::vec2(x, y);
}
