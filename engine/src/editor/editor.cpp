#define GLFW_INCLUDE_NONE

#include "jenjin/editor/editor.h"
#include "jenjin/editor/utils.h"
#include "jenjin/editor/widgets.h"
#include "jenjin/engine.h"
#include "jenjin/gameobject.h"
#include "jenjin/helpers.h"
#include "jenjin/scene.h"

#include <GLFW/glfw3.h>
#include <glm/gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include <imgui.h>
#include <imgui_internal.h>

#include <filesystem>
#include <fstream>
#include <memory>
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
          Jenjin::EngineRef->GetCurrentScene()->Load(this->paths.openScenePath);
        }

        if (ImGui::MenuItem("Save Scene")) {
          Jenjin::EngineRef->GetCurrentScene()->Save(this->paths.openScenePath);
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
          auto luaManager =
              Jenjin::EngineRef->GetCurrentScene()->GetLuaManager();

          luaManager->ReloadScripts(this->paths.projectPath + "/scripts/");
          luaManager->Ready();
        }

        ImGui::EndMenu();
      }

      if (ImGui::BeginMenu("Game")) {
        if (ImGui::MenuItem(this->running ? "Stop" : "Play")) {
          this->running = !this->running;

          auto scene = Jenjin::EngineRef->GetCurrentScene();

          if (this->running) {
            scene->Save(this->paths.liveScenePath);

            Jenjin::EngineRef->GetCurrentScene()
                ->GetLuaManager()
                ->LoadDirectory(
                    (this->paths.projectPath + "/scripts/").c_str());

            scene->GetLuaManager()->Ready();
          } else {
            scene->Load(this->paths.liveScenePath);
          }
        }

        ImGui::EndMenu();
      }
    }

    auto toDisplay = this->paths.openScenePath;
    ImGui::SetCursorPosX(ImGui::GetWindowWidth() -
                         ImGui::CalcTextSize(toDisplay.c_str()).x - 10);
    ImGui::Text("%s", toDisplay.c_str());

    ImGui::EndMainMenuBar();
  }
}

void Manager::dockspace() {
  static ImGuiDockNodeFlags dockspace_flags =
      ImGuiDockNodeFlags_PassthruCentralNode;
  static ImGuiWindowFlags window_flags =
      ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;

  // Position and resize the dockspace window
  ImGuiViewport *viewport = ImGui::GetMainViewport();
  ImGui::SetNextWindowPos(viewport->Pos);
  ImGui::SetNextWindowSize(viewport->Size);
  ImGui::SetNextWindowViewport(viewport->ID);

  // Style it so that it isn't visible, doesn't have rounding, etc
  ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
  ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
  window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
                  ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
  window_flags |=
      ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
  window_flags |= ImGuiWindowFlags_NoBackground;

  // Create the actual dockspace window
  ImGui::Begin("DockSpace", nullptr, window_flags);
  ImGui::PopStyleVar(3);

  // Create a dockspace layout
  ImGuiIO &io = ImGui::GetIO();
  ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
  ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

  static auto first_time = true;
  if (first_time) {
    first_time = false;

    // Reset layout
    ImGui::DockBuilderRemoveNode(dockspace_id);
    ImGui::DockBuilderAddNode(dockspace_id,
                              dockspace_flags | ImGuiDockNodeFlags_DockSpace);
    ImGui::DockBuilderSetNodeSize(dockspace_id, viewport->Size);

    // Dock the welcome window in the center
    ImGui::DockBuilderDockWindow("Welcome", dockspace_id);

    auto dock_left = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Left,
                                                 0.25f, nullptr, &dockspace_id);
    auto dock_left_up = ImGui::DockBuilderSplitNode(dock_left, ImGuiDir_Up,
                                                    0.8f, nullptr, &dock_left);
    auto dock_right = ImGui::DockBuilderSplitNode(dockspace_id, ImGuiDir_Right,
                                                  0.6f, nullptr, &dockspace_id);
    auto middle = dockspace_id;

    ImGui::DockBuilderDockWindow("Hierarchy", dock_left_up);
    ImGui::DockBuilderDockWindow("Explorer", dock_left);
    ImGui::DockBuilderDockWindow("Inspector", dock_right);
    ImGui::DockBuilderDockWindow("Viewport", middle);

    ImGui::DockBuilderFinish(dockspace_id);
  }

  ImGui::End();
}

void Manager::hierarchy(Jenjin::Scene *scene) {
  if (scene == nullptr) {
    spdlog::error("Scene is nullptr");
    return;
  }

  std::vector<std::shared_ptr<Jenjin::GameObject>> *gameObjects =
      Jenjin::EngineRef->GetCurrentScene()->GetGameObjects();

  ImGui::Begin("Hierarchy");

  static char name[128] = {0};
  // Press or Ctrl + Shift + A
  if (ImGui::Button("+", ImVec2(40, 40)) ||
      (ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_A)) &&
       ImGui::GetIO().KeyCtrl && ImGui::GetIO().KeyShift)) {
    memset(name, 0, sizeof(name));
    ImGui::OpenPopup("Add GameObject");
  }

  ImGui::Separator();
  ImGui::Spacing();

  ImGui::SetNextWindowSize(ImVec2(300, 200));
  if (ImGui::BeginPopupModal("Add GameObject")) {
    ImGui::SetItemDefaultFocus();
    ImGui::InputText("Name", name, sizeof(name));

    if (ImGui::Button("Create##Add") ||
        ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
      auto gameObject = std::make_shared<Jenjin::GameObject>(
          name, Jenjin::Helpers::CreateQuad(2.0f, 2.0f));
      scene->AddGameObject(gameObject);
      scene->Build();

      selectedCamera = false;
      selectedObject = gameObjects->back().get();

      memset(this->renameGameObjectBuffer, 0,
             sizeof(this->renameGameObjectBuffer));
      memcpy(this->renameGameObjectBuffer, selectedObject->name.c_str(),
             selectedObject->name.size());

      ImGui::CloseCurrentPopup();
    }

    ImGui::SameLine();

    if (ImGui::Button("Cancel##Add") ||
        ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }

  if (ImGui::Selectable("Camera", selectedCamera)) {
    selectedCamera = !selectedCamera;
    if (selectedCamera)
      selectedObject = nullptr;
  }

  for (std::shared_ptr<Jenjin::GameObject> gameObject : *gameObjects) {
    bool isSelected = selectedObject == gameObject.get();

    if (ImGui::Selectable(gameObject->name.c_str(), isSelected)) {
      if (isSelected) {
        selectedObject = nullptr;
      } else {
        selectedCamera = false;
        selectedObject = gameObject.get();
        memset(this->renameGameObjectBuffer, 0,
               sizeof(this->renameGameObjectBuffer));
        memcpy(this->renameGameObjectBuffer, selectedObject->name.c_str(),
               selectedObject->name.size());
      }
    }
  }

  ImGui::End();
}

void Manager::inspector(Jenjin::Scene *scene) {
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

    ImGui::DragFloat("Zoom", scene->GetCamera()->GetZoomPointer(), 0.01f, 0.01f,
                     1000.0f);

    ImGui::Unindent();

    ImGui::Text("Position");
    ImGui::Separator();
    ImGui::Indent();

    ImGui::DragFloat2("Position",
                      glm::value_ptr(*scene->GetCamera()->GetPositionPointer()),
                      0.1f);
    ImGui::DragFloat("Rotation", scene->GetCamera()->GetRotationPointer(),
                     0.1f);

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

  if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::Indent();
    Jenjin::Editor::Widgets::transformWidget(&selectedObject->transform);
    ImGui::Unindent();
  }

  ImGui::ItemSize(ImVec2(0, 10));

  if (ImGui::CollapsingHeader("Appearance", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::Indent();
    ImGui::ColorPicker3("Color", glm::value_ptr(selectedObject->color));
    ImGui::Unindent();
  }

  ImGui::ItemSize(ImVec2(0, 10));

  if (ImGui::CollapsingHeader("Textures", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::Indent();
    auto diriter = std::filesystem::directory_iterator(this->paths.projectPath +
                                                       "/textures/");
    if (diriter == std::filesystem::directory_iterator()) {
      ImGui::Text("No textures found");
    }
    for (auto &texture : diriter) {
      if (texture.is_regular_file() && texture.path().extension() == ".png" ||
          texture.path().extension() == ".jpg") {
        bool isSelected =
            selectedObject->texturePath == texture.path().string();
        if (ImGui::Selectable(texture.path().filename().string().c_str(),
                              isSelected)) {
          if (isSelected) {
            scene->SetGameObjectTexture(selectedObject, "");
          } else {
            scene->SetGameObjectTexture(selectedObject,
                                        texture.path().string());
          }
        }
      }
    }

    ImGui::ItemSize(ImVec2(0, 2));

    if (!selectedObject->texturePath.empty()) {
      ImGui::Spacing();
      ImGui::Checkbox("Mix Color", &selectedObject->mixColor);
    }

    ImGui::Unindent();
  }

  ImGui::ItemSize(ImVec2(0, 10));

  if (ImGui::CollapsingHeader("Manage", ImGuiTreeNodeFlags_DefaultOpen)) {
    ImGui::Indent();
    ImGui::InputText("##RenameInput", renameGameObjectBuffer,
                     sizeof(renameGameObjectBuffer));
    ImGui::SameLine();
    if (ImGui::Button("Rename")) {
      selectedObject->SetName(renameGameObjectBuffer);
    }
    if (ImGui::Button("Delete")) {
      scene->RemoveGameObject(selectedObject);
      selectedObject = nullptr;
    }
    ImGui::Unindent();
  }

  ImGui::Unindent();

  ImGui::End();
}

void Manager::explorer(Jenjin::Scene *scene) {
  ImGui::Begin("Explorer");

  for (auto file :
       std::filesystem::directory_iterator(this->paths.projectPath)) {
    if (file.is_directory()) {
      if (ImGui::TreeNode(file.path().filename().string().c_str())) {
        for (auto subfile : std::filesystem::directory_iterator(file.path())) {
          if (subfile.is_regular_file()) {
            ImGui::Selectable(
                fmt::format("{}", subfile.path().filename().string()).c_str());
          }
        }
        ImGui::TreePop();
      }
    }
  }

  ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 256);

  static bool demo_tools;
  ImGui::Checkbox("Demo Tools", &demo_tools);
  if (demo_tools)
    ImGui::ShowDemoWindow(&demo_tools);

  ImGui::End();
}

void Manager::backup_prompts(Jenjin::Scene *scene) {
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
      Jenjin::EngineRef->GetCurrentScene()->Save(this->paths.openScenePath);
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

void Manager::code(Jenjin::Scene *scene) {}

void Manager::show_all(Jenjin::Scene *scene) {
  bool isRunning = this->running;
  if (isRunning) {
    scene->Update();

    auto bg = ImGui::GetStyle().Colors[ImGuiCol_WindowBg];
    auto red_bg = ImVec4((bg.x + 0.01) * 1.5f, bg.y, bg.z, bg.w);
    auto red_bg_dark =
        ImVec4((bg.x + 0.01) * 1.5, bg.y * 0.9, bg.z * 0.9, bg.w);
    ImGui::PushStyleColor(ImGuiCol_WindowBg, red_bg);
    ImGui::PushStyleColor(ImGuiCol_TitleBg, red_bg_dark);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, red_bg);
  }

  if (this->paths.projectPath.empty()) {
    menu();
    dockspace();
    welcome();

    return;
  }

  menu();
  dockspace();
  if (isRunning)
    ImGui::PopStyleColor(3);

  hierarchy(scene);
  inspector(scene);
  backup_prompts(scene);
  explorer(scene);
}

void Manager::welcome() {
  ImGui::Begin("Welcome", nullptr,
               ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
                   ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar);

  static std::string selectedPreExisting = "";

  int pad = ImGui::GetStyle().WindowPadding.y * 2 -
            ImGui::GetFrameHeightWithSpacing();
  int spad = ImGui::GetStyle().WindowPadding.y * 2 +
             ImGui::GetStyle().ItemSpacing.y * 2;

  ImGui::BeginChild(
      "WelcomeChild",
      ImVec2(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2,
             ImGui::GetWindowHeight() - pad - 64 - spad -
                 ImGui::GetStyle().WindowPadding.y),
      true);

  auto open_project = [&]() {
    auto file = std::string(selectedPreExisting);
    auto jendir = Jenjin::Editor::get_jendir();
    this->paths.projectPath = jendir + "/Projects/" + file;
    this->paths.openScenePath = jendir + "/Projects/" + file + "/main.jenscene";
    this->paths.liveScenePath = jendir + "/Projects/" + file + "/live.jenscene";

    Jenjin::EngineRef->GetCurrentScene()->GetGameObjects()->clear();
    std::ifstream ifile(this->paths.openScenePath);
    Jenjin::EngineRef->GetCurrentScene()->Load(this->paths.openScenePath);

    // Load all the lua files
    Jenjin::EngineRef->GetCurrentScene()->GetLuaManager()->LoadDirectory(
        (this->paths.projectPath + "/scripts/").c_str());
  };

  for (auto file : std::filesystem::directory_iterator(
           Jenjin::Editor::get_jendir() + "/Projects")) {
    if (file.is_directory()) {
      bool isSelected = selectedPreExisting == file.path().filename().string();
      if (ImGui::Selectable(file.path().filename().string().c_str(), isSelected,
                            ImGuiSelectableFlags_None,
                            ImVec2(ImGui::GetWindowWidth() -
                                       ImGui::GetStyle().WindowPadding.x * 2,
                                   0))) {
        selectedPreExisting = file.path().filename().string();

        if (isSelected) {
          open_project();
        }
      }
    }
  }

  ImGui::EndChild();

  ImGui::BeginChild(
      "CreateProject",
      ImVec2(ImGui::GetWindowWidth() - ImGui::GetStyle().WindowPadding.x * 2,
             64 - spad),
      true);

  if (selectedPreExisting.empty()) {
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered,
                          ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive,
                          ImVec4(0.5f, 0.5f, 0.5f, 1.0f));
  }

  if (ImGui::Button("Open Project") && !selectedPreExisting.empty()) {
    open_project();
  }
  if (selectedPreExisting.empty())
    ImGui::PopStyleColor(3);

  ImGui::SameLine();

  if (ImGui::Button("New Project")) {
    ImGui::OpenPopup("New Project");
  }

  ImGui::SameLine();

  auto text = "Delete";
  auto width = ImGui::CalcTextSize(text).x;
  ImGui::SetCursorPosX(ImGui::GetWindowWidth() - width -
                       ImGui::GetStyle().WindowPadding.x -
                       ImGui::GetStyle().ItemSpacing.x);
  if (ImGui::Button(text)) {
    ImGui::OpenPopup("DeleteProject");
  }

  if (ImGui::BeginPopupModal("New Project", NULL,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
    static char projectName[128] = {0};
    ImGui::InputText("Project Name", projectName, sizeof(projectName));
    if (ImGui::Button("Create##NewProject") ||
        ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Enter))) {
      std::string jendir = Jenjin::Editor::get_jendir();
      std::string projectPath = jendir + "/Projects/" + projectName;

      Jenjin::Editor::ensure_dir(projectPath);
      Jenjin::Editor::ensure_dir(projectPath + "/scripts");
      Jenjin::Editor::ensure_dir(projectPath + "/textures");

      this->paths.projectPath = projectPath;
      this->paths.openScenePath = projectPath + "/main.jenscene";
      this->paths.liveScenePath = projectPath + "/live.jenscene";
      Jenjin::EngineRef->GetCurrentScene()->GetGameObjects()->clear();
      Jenjin::EngineRef->GetCurrentScene()->Save(this->paths.openScenePath);
      ImGui::CloseCurrentPopup();
    }
    ImGui::SameLine();
    if (ImGui::Button("Cancel##NewProject") ||
        ImGui::IsKeyPressed(ImGui::GetKeyIndex(ImGuiKey_Escape))) {
      ImGui::CloseCurrentPopup();
    }

    ImGui::EndPopup();
  }

  if (ImGui::BeginPopupModal("DeleteProject", NULL,
                             ImGuiWindowFlags_AlwaysAutoResize)) {
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
