#include "editor.h"
#include "GLFW/glfw3.h"
#include "engine.h"
#include "scene.h"

#include <imgui_internal.h>
#include <imgui.h>

#include <gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

using namespace JenjinEditor;

Editor::Editor() {}

void Editor::menu() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("New Scene")) {
				auto scene = new Jenjin::Scene("new.jenscene");
				Jenjin::Engine->add_scene(scene, true);
			}

			if (ImGui::MenuItem("Open Scene")) {
				Jenjin::Engine->active_scene->m_game_objects.clear();
				std::ifstream file("test.jenscene"); // TODO: Get based of project state
				Jenjin::Engine->active_scene->load(file);
			}

			if (ImGui::MenuItem("Save Scene")) {
				std::ofstream file("test.jenscene"); // TODO: Get based of project state
				Jenjin::Engine->active_scene->save(file);
			}

			if (ImGui::MenuItem("Exit")) {
				Jenjin::Engine->running = false;
				Jenjin::Engine->editor = false;
				glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Scripts")) {
			if (ImGui::MenuItem("Reload")) {
				Jenjin::Engine->active_scene->reload_lua();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Game")) {
			const std::string playButton = Jenjin::Engine->editor ? "Play" : "Stop";
			if (ImGui::MenuItem(playButton.c_str())) {
				// Pointers can change when reloading the scene... we should set the selectedObject to nullptr
				selectedObject = nullptr;
				// if it was pressed and we are in the editor, save the scene and disable editing mode
				if (Jenjin::Engine->editor) {
					std::ofstream file("live.jenscene"); // TODO: Get based of project state
					Jenjin::Engine->active_scene->save(file);
					Jenjin::Engine->editor = false;
				} else {
					std::ifstream file("live.jenscene"); // TODO: Get based of project state
					Jenjin::Engine->active_scene->m_game_objects.clear();
					Jenjin::Engine->active_scene->load(file);
					Jenjin::Engine->active_scene->reload_lua();
					Jenjin::Engine->editor = true;
				}
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}

void Editor::dockspace() {
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	static ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

	// Position and resize the dockspace window
	ImGuiViewport* viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(viewport->Pos);
	ImGui::SetNextWindowSize(viewport->Size);
	ImGui::SetNextWindowViewport(viewport->ID);

	// Style it so that it isn't visible, doesn't have rounding, etc
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	window_flags |= ImGuiWindowFlags_NoBackground;

	// Create the actual dockspace window
	ImGui::Begin("DockSpace", nullptr, window_flags);
	ImGui::PopStyleVar(3);

	// Create a dockspace layout
	ImGuiIO& io = ImGui::GetIO();
	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	static auto first_time = true;
	if (first_time) {
		first_time = false;

		// Reset layout
		ImGui::DockBuilderRemoveNode(dockspace_id);
		ImGui::DockBuilderAddNode(dockspace_id, dockspace_flags | ImGuiDockNodeFlags_DockSpace);
		ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

		// Split into thirds (horizontally, so we can have a left, middle, and right)
		// |     1     |
		// |-----------|
		// | 2 | 3 | 4 |
		// |	 |   |   |
		// |	 |   |   |
		// |-----------|
		// |     5 	   |
		auto dock_id_down = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Down, 0.2f, nullptr, &dockspace_id);
		auto dock_id_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left, 0.2f, nullptr, &dockspace_id);
		auto dock_id_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right, 0.6f, nullptr, &dockspace_id);
		auto dock_id_middle = dockspace_id;

		ImGui::DockBuilderDockWindow("Explorer", dock_id_down);
		ImGui::DockBuilderDockWindow("Hierarchy", dock_id_left);
		ImGui::DockBuilderDockWindow("Inspector", dock_id_right);

		// Put viewport and scene in the same dockspace
		ImGui::DockBuilderDockWindow("Viewport", dock_id_middle);
		ImGui::DockBuilderDockWindow("Scene", dock_id_middle);
		ImGui::DockBuilderDockWindow("Code", dock_id_middle);

		ImGui::DockBuilderFinish(dockspace_id);
	}

	ImGui::End();
}

void Editor::hierarchy(Jenjin::Scene* scene) {
	if (scene == nullptr) {
		spdlog::error("Scene is nullptr");
		return;
	}

	std::vector<std::shared_ptr<Jenjin::GameObject>>& gameObjects = scene->m_game_objects;

	ImGui::Begin("Hierarchy");

	static char name[128] = {0};
	// Press or Ctrl + Shift + A
	if (ImGui::Button("+", ImVec2(40, 40)) || (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_A)) && ImGui::GetIO().KeyCtrl && ImGui::GetIO().KeyShift)) {
		memset(name, 0, sizeof(name));
		ImGui::OpenPopup("Add GameObject");
	}

	ImGui::Separator();
	ImGui::Spacing();

	ImGui::SetNextWindowSize(ImVec2(300, 200));
	if (ImGui::BeginPopupModal("Add GameObject")) {
		ImGui::SetItemDefaultFocus();
		ImGui::InputText("Name", name, sizeof(name));

		if (ImGui::Button("Create##Add") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
			auto gameObject = std::make_shared<Jenjin::GameObject>(name);
			scene->add_gameobject(gameObject);
			scene->build();

			selectedCamera = false;
			selectedObject = gameObjects.back().get();

			memset(this->renameGameObjectBuffer, 0, sizeof(this->renameGameObjectBuffer));
			memcpy(this->renameGameObjectBuffer, selectedObject->name.c_str(), selectedObject->name.size());

			/* memcpy(this->renameGameObjectBuffer, name, sizeof(name)); */
			/* spdlog::debug("{}", renameGameObjectBuffer); */

			ImGui::CloseCurrentPopup();
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel##Add") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::Selectable("Camera", selectedCamera)) {
		selectedCamera = !selectedCamera;
		if (selectedCamera) selectedObject = nullptr;
	}

	for (auto& gameObject : gameObjects) {
		bool isSelected = selectedObject == gameObject.get();

		if (ImGui::Selectable(gameObject->name.c_str(), isSelected)) {
			selectedCamera = false;
			selectedObject = gameObject.get();
			memset(this->renameGameObjectBuffer, 0, sizeof(this->renameGameObjectBuffer));
			memcpy(this->renameGameObjectBuffer, selectedObject->name.c_str(), selectedObject->name.size());
			/* spdlog::debug("{}", selectedObject->name.c_str()); */
			/* memcpy(this->renameGameObjectBuffer, selectedObject->name.c_str(), selectedObject->name.size()); */
			/* spdlog::debug("{}", renameGameObjectBuffer); */
		}
	}

	ImGui::End();
}

void Editor::inspector(Jenjin::Scene* scene) {
	if (!selectedObject && !selectedCamera) {
		return;
	}

	ImGui::Begin("Inspector");

	if (selectedCamera) {
		ImGui::Text("Camera");
		ImGui::Separator();
		ImGui::Indent();

		ImGui::Text("Projection");
		ImGui::Separator();
		ImGui::Indent();

		static float zoom = 1.0f;
		if (scene->m_default_camera.zoom != 0.0f) {
			zoom = 1.0f / scene->m_default_camera.zoom;
		}
		if (ImGui::DragFloat("Zoom", &zoom, 0.01f, 0.01f, 10.0f))
			scene->m_default_camera.zoom = 1.0f / zoom;

		ImGui::Unindent();

		ImGui::Text("Position");
		ImGui::Separator();
		ImGui::Indent();

		ImGui::DragFloat2("Position", glm::value_ptr(scene->m_default_camera.position), 0.1f);
		ImGui::DragFloat("Rotation", &scene->m_default_camera.rotation, 0.1f);

		ImGui::Unindent();

		ImGui::Text("Appearance");
		ImGui::Separator();
		ImGui::Indent();

		ImGui::ColorEdit3("Clear Color", glm::value_ptr(scene->m_default_camera.clearColor));

		ImGui::Unindent();

		ImGui::Unindent();
		ImGui::End();
		return;
	}

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

	ImGui::Text("Transform");
	ImGui::Separator();
	ImGui::Indent();

	ImGui::DragFloat2("Position", glm::value_ptr(selectedObject->transform.position), 0.1f);
	ImGui::DragFloat("Rotation", &selectedObject->transform.rotation, 0.1f);
	ImGui::DragFloat2("Scale", glm::value_ptr(selectedObject->transform.scale), 0.025f);

	ImGui::Unindent();

	ImGui::Text("Appearance");
	ImGui::Separator();
	ImGui::Indent();

	ImGui::Text("Colours");
	ImGui::Separator();
	ImGui::Indent();
	ImGui::Spacing();
	ImGui::ColorEdit3("Color", glm::value_ptr(selectedObject->color));
	ImGui::Unindent();

	ImGui::Spacing();

	ImGui::Text("Textures");
	ImGui::Separator();
	ImGui::Indent();
	ImGui::Spacing();
	for (auto file : std::filesystem::directory_iterator("engine/resources/textures")) {
		if (file.path().extension() != ".png" && file.path().extension() != ".jpg") {
			continue;
		}

		bool isSelected = selectedObject->texture_path == file.path().string();
		if (ImGui::Selectable(file.path().filename().string().c_str(), isSelected)) {
			if (isSelected) {
				selectedObject->remove_texture();
			} else {
				spdlog::debug("Selected texture: {}", file.path().string());
				selectedObject->set_texture(file.path().string(), file.path().extension() == ".png");
			}
		}
	}
	ImGui::Unindent();
	ImGui::Unindent();

	ImGui::Text("Manage");
	ImGui::Separator();
	ImGui::Indent();
	ImGui::Spacing();

	ImGui::InputText("##RenameInput", renameGameObjectBuffer, sizeof(renameGameObjectBuffer));
	ImGui::SameLine();
	if (ImGui::Button("Rename")) {
		selectedObject->set_name(renameGameObjectBuffer);
	}

	if (ImGui::Button("Delete")) {
		scene->remove_gameobject(selectedObject);
	}

	ImGui::Unindent();
	ImGui::Unindent();

	ImGui::End();
}

void Editor::scene(Jenjin::Scene* scene) {
	if (ImGui::Begin("Scene")) {
		renderableWindow = Scene;
		renderableWindowSize = ImGui::GetContentRegionAvail();
	}

	ImVec2 size = ImGui::GetContentRegionAvail();
	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	draw_list->AddImage(
		(void*)(intptr_t)Jenjin::Engine->framebuffer.texture,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + size.x,
				 ImGui::GetCursorScreenPos().y + size.y),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);

	// rect in top right corner as bg for zoom level text
	draw_list->AddRectFilled(ImVec2(ImGui::GetCursorScreenPos().x + size.x - 100, ImGui::GetCursorScreenPos().y),
													ImVec2(ImGui::GetCursorScreenPos().x + size.x, ImGui::GetCursorScreenPos().y + 20),
													IM_COL32(0, 0, 0, 128));

	char buffer[64];
	snprintf(buffer, sizeof(buffer), "Zoom: %.2fx", 1.0f / scene->m_editor_camera.zoom);
	draw_list->AddText(ImVec2(ImGui::GetCursorScreenPos().x + size.x - 100, ImGui::GetCursorScreenPos().y), IM_COL32(255, 255, 255, 255), buffer);

	ImGui::End();
}

void Editor::viewport(Jenjin::Scene* scene) {
	if (ImGui::Begin("Viewport")) {
		renderableWindow = Viewport;
		renderableWindowSize = ImGui::GetContentRegionAvail();
	}

	ImVec2 size = ImGui::GetContentRegionAvail();

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

void Editor::explorer(Jenjin::Scene* scene) {
	ImGui::Begin("Explorer");

	static bool demo_tools; ImGui::Checkbox("Demo Tools", &demo_tools); if (demo_tools) ImGui::ShowDemoWindow(&demo_tools);
	static bool jenjin_demo; ImGui::Checkbox("Jenjin Demo", &jenjin_demo);

	if (jenjin_demo) {
		static float hue = 0.0f;

		auto change_ui_hue = [](float hue) {
			ImGuiStyle& style = ImGui::GetStyle();
			for (int i = 0; i < ImGuiCol_COUNT; i++) {
			static ImVec4* colors = style.Colors;
			float h, s, v; ImGui::ColorConvertRGBtoHSV(colors[i].x, colors[i].y, colors[i].z, h, s, v);
			float r, g, b; ImGui::ColorConvertHSVtoRGB(hue, s, v, r, g, b);
			colors[i] = ImVec4(r, g, b, colors[i].w);
			}
		};

		auto change_camera_hue = [](float hue) {
			glm::vec3* clearColor = &Jenjin::Engine->active_scene->m_default_camera.clearColor;
			float h, s, v; ImGui::ColorConvertRGBtoHSV(clearColor->r, clearColor->g, clearColor->b, h, s, v);
			h = hue;
			float r, g, b; ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
			memcpy(glm::value_ptr(*clearColor), glm::value_ptr(glm::vec3(r, g, b)), sizeof(glm::vec3));
		};

		float ch, cs, cv;
		ch = 0.0f; cs = 1.0f; cv = 1.0f;
		float cr, cg, cb;
		ImGui::ColorConvertHSVtoRGB(hue, cs, cv, cr, cg, cb);
		static ImVec4 col = ImVec4(cr, cg, cb, 1.0f);
		if (ImGui::ColorEdit3("Base colour", (float*)&col)) {
			float nh, ns, nv;
			ImGui::ColorConvertRGBtoHSV(col.x, col.y, col.z, nh, ns, nv);
			hue = nh;
			change_ui_hue(hue);
		}

		if (ImGui::DragFloat("Hue", &hue, 0.001f, 0.0f, 1.0f)) {
			change_ui_hue(hue);
		}

		static bool rainbow; ImGui::Checkbox("Rainbow", &rainbow);
		if (rainbow) {
			hue = fmodf(hue + 0.006f, 1.0f);
			change_ui_hue(hue);
			change_camera_hue(hue);
		}
	}

	ImGui::End();
}

void Editor::backup_prompts(Jenjin::Scene* scene) {
	if (glfwWindowShouldClose(glfwGetCurrentContext())) {
		if (!ImGui::IsPopupOpen("Exit")) {
			glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_FALSE);
			ImGui::OpenPopup("Exit");
		}
	}

	// promtp with exit, save and exit, cancel
	if (ImGui::BeginPopupModal("Exit", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Do you want to save before exiting?");
		ImGui::Separator();

		if (ImGui::Button("Save and Exit")) {
			std::ofstream file("test.jenscene"); // TODO: Get based of project state
			Jenjin::Engine->active_scene->save(file);
			Jenjin::Engine->running = false;
			glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
		}

		ImGui::SameLine();

		if (ImGui::Button("Exit")) {
			Jenjin::Engine->running = false;
			glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
		}

		ImGui::SameLine();

		if (ImGui::Button("Cancel")) {
			glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_FALSE);
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}
}

void Editor::code(Jenjin::Scene* scene) {
	ImGui::Begin("Code");

	ImGui::Text("Put ZEP embedded code editor here");

	ImGui::End();
}

void Editor::show_all(Jenjin::Scene* scene) {
	menu();
	dockspace();

	bool in_editor = Jenjin::Engine->editor;
	if (!in_editor) {
		ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.2f, 0.05f, 0.05f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_TitleBg, ImVec4(0.239f, 0.082f, 0.082f, 0.94f));
		ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.339f, 0.182f, 0.182f, 0.94f));
	}

	hierarchy(scene);
	inspector(scene);
	explorer(scene);
	code(scene);

	Jenjin::Engine->render_into_imgui(renderableWindowSize.x, renderableWindowSize.y, renderableWindow == Viewport);

	viewport(scene);
	this->scene(scene);

	if (!in_editor) {
		ImGui::PopStyleColor(3);
	}

	backup_prompts(scene);
}
