#include "engine.h"
#include "gameobject.h"
#include "gtc/type_ptr.hpp"
#include "scene.h"

#include <imgui.h>

#include <filesystem>
#include <fstream>
#include <memory>

void header(std::string title) {
	ImGui::Spacing();
	ImGui::Text("%s", title.c_str());
	ImGui::Separator();
	ImGui::Spacing();
}

void save(Jenjin::Scene* scene) {
	std::ofstream ose("editor.jenscene");
	std::ofstream ost("test.jenscene");
	scene->save(ose);
	scene->save(ost);
}

std::string getFilename(const std::string& fullPath) {
    auto lastSlashPos = fullPath.find_last_of('/');

    if (lastSlashPos == std::string::npos) {
        return fullPath;
    }

    return fullPath.substr(lastSlashPos + 1);
}

void texture_picker(Jenjin::Scene* scene, std::shared_ptr<Jenjin::GameObject>& gobj) {
	ImGui::Text("Texture %s", gobj->texture_path.empty() ? "None" : getFilename(gobj->texture_path).c_str());

	ImGui::SameLine();
	if (ImGui::SmallButton("Change")) {
		auto* path = new char[256];
		ImGui::OpenPopup("Texture picker");
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Remove")) {
		gobj->remove_texture();
	}

	if (ImGui::BeginPopup("Texture picker")) {
		header("Texture picker");

		const char* path = "test/assets/";
		for (auto& p : std::filesystem::directory_iterator(path)) {
			const std::string ext = p.path().extension().string();
			if (ext != ".png" && ext != ".jpg" && ext != ".jpeg")
				continue;

			if (ImGui::Selectable(p.path().filename().string().c_str())) {
				gobj->set_texture(p.path().string(), ext == ".png");
			}
		}

		ImGui::EndPopup();
	}
}

int main(void) {
	auto* scene = new Jenjin::Scene();
	scene->load(*new std::ifstream("editor.jenscene"));

	scene->set_render_callback([](Jenjin::Scene* scene) {
		ImGui::Begin("Game objects");

		if (ImGui::Button("+", ImVec2(50, 50))) {
			ImGui::OpenPopup("Add GameObject");
		}

		ImGui::SetNextWindowSize(ImVec2(200, 100));
		if (ImGui::BeginPopupModal("Add GameObject")) {
			static char name[128];
			ImGui::SetKeyboardFocusHere();
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

				ImGui::TreePop();
				ImGui::PopID();
			}
		}

		ImGui::End();
	});

	Jenjin::Engine.add_scene(scene, true);
	Jenjin::Engine.launch(1920, 1080, "Jenjin Editor");
}
