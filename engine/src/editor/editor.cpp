#include "jenjin/editor/editor.h"
#include "jenjin/editor/utils.h"
#include "jenjin/engine.h"
#include "jenjin/gameobject.h"
#include "jenjin/helpers.h"
#include "jenjin/scene.h"

#include <imgui_internal.h>
#include <imgui.h>

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

using namespace Jenjin::Editor;

Manager::Manager() {}

void Manager::menu() {
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (!this->paths.projectPath.empty()) {
				if (ImGui::MenuItem("New Scene")) {
					auto scene = std::make_shared<Jenjin::Scene>();
					Jenjin::EngineRef->AddScene(scene, true);
				}

				if (ImGui::MenuItem("Open Scene")) {
					std::ifstream file(this->paths.openScenePath);
					spdlog::warn("Unimplemented: Open Scene");
				}

				if (ImGui::MenuItem("Save Scene")) {
					std::ofstream file(this->paths.openScenePath);
					spdlog::warn("Unimplemented: Save Scene");
				}
			}

			if (ImGui::MenuItem("Exit")) {
				glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
			}

			ImGui::EndMenu();
		}

		if (!this->paths.openScenePath.empty()) {
			if (ImGui::BeginMenu("Scripts")) {
				if (ImGui::MenuItem("Reload")) {
					spdlog::warn("Unimplemented: Reload Scripts");
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Game")) {
				if (ImGui::MenuItem("Play")) {
						spdlog::warn("Unimplemented: Play");
				}

				ImGui::EndMenu();
			}
		}

		auto toDisplay = this->paths.openScenePath;
		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - ImGui::CalcTextSize(toDisplay.c_str()).x - 10);
		ImGui::Text("%s", toDisplay.c_str());

		ImGui::EndMainMenuBar();
	}
}

void Manager::dockspace() {
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

		// Dock the welcome window in the center
		ImGui::DockBuilderDockWindow("Welcome", dockspace_id);

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

void Manager::hierarchy(Jenjin::Scene* scene) {
	if (scene == nullptr) {
		spdlog::error("Scene is nullptr");
		return;
	}

	std::vector<std::shared_ptr<Jenjin::GameObject>>* gameObjects = Jenjin::EngineRef->GetCurrentScene()->GetGameObjects();

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
			auto gameObject = std::make_shared<Jenjin::GameObject>(name, Jenjin::Helpers::CreateQuad(2.0f, 2.0f));
			scene->AddGameObject(gameObject);
			scene->Build();

			selectedCamera = false;
			selectedObject = gameObjects->back().get();

			memset(this->renameGameObjectBuffer, 0, sizeof(this->renameGameObjectBuffer));
			memcpy(this->renameGameObjectBuffer, selectedObject->name.c_str(), selectedObject->name.size());

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

	for (std::shared_ptr<Jenjin::GameObject> gameObject : *gameObjects) {
		bool isSelected = selectedObject == gameObject.get();

		if (ImGui::Selectable(gameObject->name.c_str(), isSelected)) {
			if (isSelected) {
				selectedObject = nullptr;
			} else {
				selectedCamera = false;
				selectedObject = gameObject.get();
				memset(this->renameGameObjectBuffer, 0, sizeof(this->renameGameObjectBuffer));
				memcpy(this->renameGameObjectBuffer, selectedObject->name.c_str(), selectedObject->name.size());
			}
		}
	}

	ImGui::End();
}

void Manager::inspector(Jenjin::Scene* scene) {
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

		ImGui::DragFloat("Zoom", scene->GetCamera()->GetZoomPointer(), 0.01f, 0.01f, 1000.0f);

		ImGui::Unindent();

		ImGui::Text("Position");
		ImGui::Separator();
		ImGui::Indent();

		ImGui::DragFloat2("Position", glm::value_ptr(*scene->GetCamera()->GetPositionPointer()), 0.1f);
		ImGui::DragFloat("Rotation", scene->GetCamera()->GetRotationPointer(), 0.1f);

		ImGui::Unindent();

		ImGui::Text("Appearance");
		ImGui::Separator();
		ImGui::Indent();

		ImGui::Text("Color here...");

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

	ImGui::Text("Manage");
	ImGui::Separator();
	ImGui::Indent();
	ImGui::Spacing();

	ImGui::InputText("##RenameInput", renameGameObjectBuffer, sizeof(renameGameObjectBuffer));
	ImGui::SameLine();
	if (ImGui::Button("Rename")) {
		selectedObject->SetName(renameGameObjectBuffer);
	}

	if (ImGui::Button("Delete")) {
		scene->RemoveGameObject(std::shared_ptr<Jenjin::GameObject>(selectedObject));
		selectedObject = nullptr;
	}

	ImGui::Unindent();
	ImGui::Unindent();

	ImGui::End();
}

void Manager::explorer(Jenjin::Scene* scene) {
	ImGui::Begin("Explorer");

	// context menu with new popup (builtinto menu that asks for name of file)
	if (ImGui::BeginPopupContextWindow()) {
		if (ImGui::BeginMenu("New")) {
			static char name[128] = {0};
			ImGui::InputText("Name", name, sizeof(name));
			if (ImGui::Button("Create")) {
				std::ofstream file(this->paths.projectPath + "/scripts/" + std::string(name));
				file << "function ready()\n	print(\"Ready!\")\nend\n\nfunction update()\n 	print(\"Update\")\nend\n";
				file.close();
				ImGui::CloseCurrentPopup();
			}
			ImGui::EndMenu();
		}

		ImGui::EndPopup();
	}

	for (auto file : std::filesystem::directory_iterator(this->paths.projectPath + "/scripts/")) {
		if (file.is_regular_file()) {
			if (ImGui::Selectable(file.path().filename().string().c_str())) {
			}
		}
	}

	ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 256);

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
		}
	}

	ImGui::End();
}

void Manager::backup_prompts(Jenjin::Scene* scene) {
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
			std::ofstream file(this->paths.openScenePath);
			spdlog::warn("Unimplemented: Save Scene");
			glfwSetWindowShouldClose(glfwGetCurrentContext(), GLFW_TRUE);
		}

		ImGui::SameLine();

		if (ImGui::Button("Exit")) {
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

void Manager::code(Jenjin::Scene* scene) {
}

void Manager::show_all(Jenjin::Scene* scene) {
	if (this->paths.projectPath.empty()) {
		menu();
		dockspace();
		welcome();

		return;
	}

	menu();
	dockspace();

	hierarchy(scene);
	inspector(scene);
	backup_prompts(scene);
}

void Manager::welcome() {
	ImGui::Begin("Welcome", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

	static std::string selectedPreExisting = "";

	int pad = ImGui::GetStyle().WindowPadding.y * 2 - ImGui::GetFrameHeightWithSpacing();
	int spad = ImGui::GetStyle().WindowPadding.y * 2 + ImGui::GetStyle().ItemSpacing.y * 2;
	ImGui::BeginChild("WelcomeChild", ImVec2(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2, ImGui::GetWindowHeight() - pad - 64 - spad - ImGui::GetStyle().WindowPadding.y), true);

	auto open_project = [&]() {
		auto file = std::string(selectedPreExisting);
		auto jendir = Jenjin::Editor::get_jendir();
		this->paths.projectPath = jendir + "/Projects/" + file;
		this->paths.openScenePath = jendir + "/Projects/" + file + "/main.jenscene";
		this->paths.liveScenePath = jendir + "/Projects/" + file + "/live.jenscene";

		Jenjin::EngineRef->GetCurrentScene()->GetGameObjects()->clear();
		std::ifstream ifile(this->paths.openScenePath);
		/* Jenjin::EngineRef->GetCurrentScene()->load(ifile); */
		spdlog::warn("Unimplemented: Load Scene");

		// Load all the lua files
		spdlog::warn("Unimplemented: Load Lua files");
		/* Jenjin::EngineRef->GetCurrentScene()->reload_lua((this->paths.projectPath + "/scripts/").c_str()); */
	};

	for (auto file : std::filesystem::directory_iterator(Jenjin::Editor::get_jendir() + "/Projects")) {
		if (file.is_directory()) {
			bool isSelected = selectedPreExisting == file.path().filename().string();
			if (ImGui::Selectable(file.path().filename().string().c_str(), isSelected, ImGuiSelectableFlags_None, ImVec2(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2, 0))) {
				selectedPreExisting = file.path().filename().string();

				if (isSelected) {
					open_project();
				}
			}
		}
	}

	ImGui::EndChild();

	ImGui::BeginChild("CreateProject", ImVec2(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2, 64 - spad), true);

	if (selectedPreExisting.empty()) {
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
	}

	if (ImGui::Button("Open Project") && !selectedPreExisting.empty()) {
		open_project();
	}
	if (selectedPreExisting.empty()) ImGui::PopStyleColor(3);

	ImGui::SameLine();

	if (ImGui::Button("New Project")) {
		ImGui::OpenPopup("NewProject");
	}

	ImGui::SameLine();

	auto text = "Delete";
	auto width = ImGui::CalcTextSize(text).x;
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() - width - ImGui::GetStyle().WindowPadding.x - ImGui::GetStyle().ItemSpacing.x);
	if (ImGui::Button(text)) {
		ImGui::OpenPopup("DeleteProject");
	}

	if (ImGui::BeginPopupModal("NewProject", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		static char projectName[128] = {0};
		ImGui::InputText("Project Name", projectName, sizeof(projectName));
		if (ImGui::Button("Create##NewProject") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
			std::string jendir = Jenjin::Editor::get_jendir();
			std::string projectPath = jendir + "/Projects/" + projectName;

			Jenjin::Editor::ensure_dir(projectPath);
			Jenjin::Editor::ensure_dir(projectPath + "/scripts");
			Jenjin::Editor::ensure_dir(projectPath + "/textures");

			this->paths.projectPath = projectPath;
			this->paths.openScenePath = projectPath + "/main.jenscene";
			this->paths.liveScenePath = projectPath + "/live.jenscene";
			Jenjin::EngineRef->GetCurrentScene()->GetGameObjects()->clear();
			/* std::ofstream ofile(this->paths.openScenePath); */
			/* Jenjin::Engine->active_scene->save(ofile); */
			spdlog::warn("Unimplemented: Save Scene");
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel##NewProject") || ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
			ImGui::CloseCurrentPopup();
		}

		ImGui::EndPopup();
	}

	if (ImGui::BeginPopupModal("DeleteProject", NULL, ImGuiWindowFlags_AlwaysAutoResize)) {
		ImGui::Text("Are you sure you want to delete this project?");
		ImGui::Separator();
		if (ImGui::Button("Delete##DeleteProject")) {
			std::string jendir = Jenjin::Editor::get_jendir();
			std::string projectPath = jendir + "/Projects/" + selectedPreExisting;
			std::filesystem::remove_all(projectPath);
			selectedPreExisting = "";
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine();
		if (ImGui::Button("Cancel##DeleteProject")) {
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
	}

	ImGui::EndChild();
	ImGui::End();
}
