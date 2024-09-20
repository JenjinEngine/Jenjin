#include "scene.h"
#include "editor.h"
#include "state.h"

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <fstream>

void jenjin_menubar(Jenjin::Scene* scene) {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Scene")) {
				scene->m_game_objects.clear();
				scene->build();
			}
			if (ImGui::MenuItem("Open Scene")) {
				std::ifstream file("test.jenscene");
				scene->load(file);
			}
			if (ImGui::MenuItem("Save Scene")) {
				std::ofstream file("test.jenscene");
				scene->save(file);
			}
			if (ImGui::MenuItem("Quit")) {
				glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
			}
			ImGui::EndMenu();
		}

		// Controls for state
		static bool running = State.running;
		ImGui::Checkbox("Running", &State.running);
		if (running != State.running) {
			running = State.running;
			if (State.running) {
				std::ofstream os("test.jenscene");
			  scene->save(os);
			} else {
				scene->m_game_objects.clear();
				std::ifstream is("test.jenscene");
				scene->load(is);
				scene->update_lua_ptrs();
			}
		}

		ImGui::EndMainMenuBar();
	}

	// Quit popup confirmation
	if (glfwWindowShouldClose(glfwGetCurrentContext())) {
		if (!ImGui::IsPopupOpen("Quit?")) {
			glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_FALSE);

			ImGui::OpenPopup("Quit?");
		}
	}

	if (ImGui::BeginPopupModal("Quit?", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Are you sure you want to quit?");
		ImGui::Separator();
		ImGui::Spacing();

		if (ImGui::Button("Yes", ImVec2(120, 0)) || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
			glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
			ImGui::CloseCurrentPopup();
		}

		ImGui::SetItemDefaultFocus();
		ImGui::SameLine();

		if (ImGui::Button("No", ImVec2(120, 0))) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
}
