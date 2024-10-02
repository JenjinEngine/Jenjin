#include "jenjin/targets/editor.h"
#include "jenjin/engine.h"
#include "jenjin/framebuffer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

using namespace Jenjin::Targets;

EditorTarget::EditorTarget() {
}

void EditorTarget::PreRender() {
	static int i = 0; if (i < 5) { i++;
		Jenjin::EngineRef->GetCurrentScene()->GetCamera()->SetPosition(glm::vec3(0, 0, 0));
	}

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	Resize(GetSize());

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	this->editor.show_all(Jenjin::EngineRef->GetCurrentScene());

	renderTexture.Bind();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glViewport(0, 0, width, height);
}

void EditorTarget::Render() {
	renderTexture.Unbind();

	if (this->editor.paths.openScenePath.empty()) {
		return;
	}

	ImGui::Begin("Viewport");

	ImVec2 size = ImGui::GetContentRegionAvail();
	this->width = size.x;
	this->height = size.y;

	ImGui::GetWindowDrawList()->AddImage(
		(void*)(intptr_t)renderTexture.texture,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + size.x,
				 ImGui::GetCursorScreenPos().y + size.y),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	std::string res_text = std::to_string((int)size.x) + "x" + std::to_string((int)size.y);
	ImGui::GetWindowDrawList()->AddText(
		ImVec2(ImGui::GetCursorScreenPos().x + 10, ImGui::GetCursorScreenPos().y + 10),
		ImColor(255, 255, 255, 255),
		res_text.c_str()
	);

	ImGui::End();
}

void EditorTarget::PostRender() {
	ImGui::EndFrame();
	ImGui::Render();

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

glm::vec2 EditorTarget::GetSize() {
	return glm::vec2(width, height);
}

void EditorTarget::Resize(glm::vec2 size) {
	this->width = size.x;
	this->height = size.y;
	renderTexture.Resize(width, height);
	Jenjin::EngineRef->GetCurrentScene()->GetCamera()->Resize(size);
}

glm::vec2 EditorTarget::GetMousePosition() {
	// WARNING: Untested...

	static auto ctx = glfwGetCurrentContext();
	static double gx, gy;
	glfwGetCursorPos(ctx, &gx, &gy);

	// g_ is the whole window
	// l_ is the viewport we need to calculate this
	auto l_ = ImGui::GetWindowPos();
	auto lx = gx - l_.x;
	auto ly = gy - l_.y;

	return glm::vec2(lx, ly);
}

// We handle window resizing ourselves
bool EditorTarget::RespondsToWindowResize() {
	return false;
}
