#include "editor.h"

#include <gtc/type_ptr.hpp>

#include <fstream>

void jenjin_explorer(Jenjin::Scene* scene) {
	ImGui::Begin("Game objects");

	if (ImGui::Button("+", ImVec2(50, 50))) {
		ImGui::OpenPopup("Add GameObject");
	}

	ImGui::SetNextWindowSize(ImVec2(200, 100));
	if (ImGui::BeginPopupModal("Add GameObject")) {
		static char name[128];
		if (ImGui::IsWindowAppearing()) ImGui::SetKeyboardFocusHere();
		ImGui::InputText("Name", name, sizeof(name));

		ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 20);

		if (ImGui::SmallButton("Create")) {
			scene->add_gameobject(std::make_shared<Jenjin::GameObject>(name));
			scene->build();
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize("Cancel").x - 20);

		if (ImGui::SmallButton("Cancel")) {
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
			ImGui::CloseCurrentPopup();
		}

		if (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
			scene->add_gameobject(std::make_shared<Jenjin::GameObject>(name));
			scene->build();
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}
	ImGui::SameLine();

	if (ImGui::Button("Clear", ImVec2(50, 50))) {
		scene->m_game_objects.clear();
		scene->build();
	}
	ImGui::SameLine();

	if (ImGui::Button("Save", ImVec2(50, 50))) {
		save(scene);
	}
	ImGui::SameLine();

	if (ImGui::Button("Load", ImVec2(50, 50))) {
		scene->m_game_objects.clear();
		std::ifstream is("editor.jenscene");
		scene->load(is);
	}

	ImGui::Spacing();
	ImGui::Separator();

	int i = 0;
	for (auto& go : scene->m_game_objects) {
		if (ImGui::TreeNode(go->name.c_str())) {
			ImGui::PushID(i++);

			header("Transform");
			ImGui::DragFloat2("Position", glm::value_ptr(go->transform.position), 0.1);
			ImGui::DragFloat("Rotation", &go->transform.rotation, 0.5);
			ImGui::DragFloat2("Scale", glm::value_ptr(go->transform.scale), 0.01);

			header("Appearance");
			ImGui::ColorPicker3("Color", glm::value_ptr(go->color));
			texture_picker(scene, go);

			header("Management");
			if (ImGui::Button("Delete")) {
				int index = 0;
				for (auto& g : scene->m_game_objects) {
					if (g->name == go->name) {
						scene->m_game_objects.erase(scene->m_game_objects.begin() + index);
						break;
					}
					index++;
				}
				scene->build();
			}

			ImGui::TreePop();
			ImGui::PopID();
		}
	}

	ImGui::End();
}
