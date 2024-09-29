#include "jenjin/engine.h"
#include "mydisplay.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <spdlog/spdlog.h>

void MyDisplay::PreRender() {
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
}

void MyDisplay::Render() {
	ImGui::Begin("Test UI");

	static Jenjin::Engine* engine = Jenjin::EngineRef;
	ImGui::DragFloat2("Camera Position", glm::value_ptr(*engine->GetCurrentScene()->GetCamera()->GetPositionPointer()), 1.0f);
	ImGui::DragFloat("Camera Zoom", engine->GetCurrentScene()->GetCamera()->GetZoomPointer(), 1.0f);

	auto gobjs = engine->GetCurrentScene()->GetGameObjects();

	int i = 0;
	for (auto& gobj : *gobjs) {
		ImGui::PushID(i++);
		if (ImGui::CollapsingHeader(gobj->GetName().c_str())) {
			ImGui::Text("%s", gobj->GetName().c_str());
			ImGui::DragFloat2("Position", glm::value_ptr(*gobj->GetPositionPointer()), 0.01f);
			ImGui::DragFloat2("Scale", glm::value_ptr(*gobj->GetScalePointer()), 0.01f);
			ImGui::DragFloat("Rotation", gobj->GetRotationPointer(), 0.01f);
		}
		ImGui::PopID();
	}

	ImGui::End();
}

void MyDisplay::PostRender() {
	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

glm::vec2 MyDisplay::GetSize() {
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
