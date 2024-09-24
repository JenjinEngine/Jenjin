#include "editor.h"
#include "engine.h"
#include "gtc/type_ptr.hpp"
#include "scene.h"

#include <imgui.h>
#include <spdlog/spdlog.h>

#include <vector>

using namespace JenjinEditor;

Editor::Editor() {}

void Editor::hierarchy(Jenjin::Scene* scene) {
	if (scene == nullptr) {
		spdlog::error("Scene is nullptr");
		return;
	}

	std::vector<std::shared_ptr<Jenjin::GameObject>>& gameObjects = scene->m_game_objects;

	static bool first = true;
	if (gameObjects.empty() && first) {
		spdlog::info("No game objects in scene");
		first = false;
		return;
	}

	ImGui::Begin("Hierarchy");

	for (auto& gameObject : gameObjects) {
		bool isSelected = selectedObject == gameObject.get();

		if (ImGui::Selectable(gameObject->name.c_str(), isSelected)) {
			selectedObject = gameObject.get();
		}
	}

	ImGui::End();
}

void Editor::inspector(Jenjin::Scene* scene) {
	ImGui::Begin("Inspector");

	if (selectedObject == nullptr) {
		ImGui::Text("No object selected");
		ImGui::End();

		return;
	}

	if (!(ImGui::CollapsingHeader(selectedObject->name.c_str()), ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Bullet)) {
		ImGui::End();
		return;
	}

	ImGui::Indent();

	// Name is already shown in the header
	// Show transform
	ImGui::Text("Transform");
	ImGui::Indent();

	ImGui::DragFloat3("Position", glm::value_ptr(selectedObject->transform.position), 0.1f);
	ImGui::DragFloat("Rotation", &selectedObject->transform.rotation, 0.1f);
	ImGui::DragFloat2("Scale", glm::value_ptr(selectedObject->transform.scale), 0.1f);

	ImGui::Unindent();

	ImGui::Text("Appearance");

	ImGui::Indent();

	ImGui::ColorPicker3("Color", glm::value_ptr(selectedObject->color));

	if (ImGui::Button("Remove texture")) {
		selectedObject->remove_texture();
	}

	ImGui::Unindent();

	ImGui::Unindent();

	ImGui::End();
}

void Editor::viewport(Jenjin::Scene* scene) {
	ImGui::Begin("Viewport");

	ImVec2 size = ImGui::GetContentRegionAvail();
	Jenjin::Engine->render_into_imgui(size.x, size.y);

	ImGui::GetWindowDrawList()->AddImage(
		(void*)(intptr_t)Jenjin::Engine->framebuffer.texture,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + size.x,
				 ImGui::GetCursorScreenPos().y + size.y),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	ImGui::End();
}

void Editor::show_all(Jenjin::Scene* scene) {
	hierarchy(scene);
	inspector(scene);
	viewport(scene);
}
