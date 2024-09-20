#include "scene.h"
#include "editor.h"
#include "state.h"

#include "engine.h"

#include <imgui.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

#include <fstream>

static bool show_debug = false;

void jenjin_menubar(Jenjin::Scene* scene) {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Scene")) {
				scene->m_game_objects.clear();
				scene->build();
			}

			if (ImGui::MenuItem("Open Scene")) {
				std::ifstream file("live.jenscene");
				scene->load(file);
			}

			if (ImGui::MenuItem("Save Scene")) {
				std::ofstream file("live.jenscene");
				scene->save(file);
			}

			if (ImGui::MenuItem("Quit")) {
				glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scripting")) {
			if (ImGui::MenuItem("Reload scripts")) {
				scene->reload_lua();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Help")) {
			if (ImGui::MenuItem("About")) {
				ImGui::OpenPopup("About");
			}

			if (ImGui::MenuItem("Debug")) {
				show_debug = !show_debug;
			}

			ImGui::EndMenu();
		}

		// Controls for state
		static bool running = State.running;
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("Running").x - 32);
		ImGui::Checkbox("Running", &State.running);
		if (running != State.running) {
			running = State.running;
			if (State.running) {
				std::ofstream os("live.jenscene");
				scene->save(os);
			} else {
				scene->m_game_objects.clear();
				std::ifstream is("live.jenscene");
				scene->load(is);
				scene->update_lua_ptrs();
			}
		}

		ImGui::EndMainMenuBar();
	}

	// About popup
	if (ImGui::BeginPopupModal("About", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Jenjin Editor");
		ImGui::Separator();
		ImGui::Spacing();
		ImGui::Text("Jenjin version %s", JENJIN_VERSION);
		ImGui::Spacing();
		ImGui::EndPopup();
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

	if (show_debug) {
		ImGui::Begin("Debug", &show_debug);

		// NOTE: Technically this is not accurate, but it's close enough due to flickering being annoying
		ImGui::Text("FPS: %d", (int)ImGui::GetIO().Framerate == 59 ? 60 : (int)ImGui::GetIO().Framerate);

		ImGui::Text("Frame Time: %f", ImGui::GetIO().DeltaTime);
		ImGui::Text("Running: %s", State.running ? "true" : "false");
		ImGui::End();
	}
}
