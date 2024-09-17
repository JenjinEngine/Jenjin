#include "editor.h"

#include <GLFW/glfw3.h>

int jenjin_dockspace() {
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	int w, h; glfwGetWindowSize(glfwGetCurrentContext(), &w, &h);
	ImGui::SetNextWindowSize(ImVec2(w, h));
	ImGui::Begin("DockSpace", nullptr, ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiDockNodeFlags_PassthruCentralNode | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus);
	static ImGuiID dockspace_id = ImGui::GetID("DockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);

	return dockspace_id;
}
