#define GLFW_INCLUDE_NONE

#include "jenjin/editor/utils.h"
#include "jenjin/targets/editor.h"
#include "jenjin/targets/runtime.h"

#include "jenjin/engine.h"
#include "jenjin/helpers.h"
#include "jenjin/scene.h"

#include <spdlog/spdlog.h>

#include <filesystem>
#include <cstdlib>
#include <memory>

#define LAUNCH_ARGS                                                            \
  Jenjin::Engine &engine, GLFWwindow *window,                                  \
      std::shared_ptr<Jenjin::Scene> scene

void launchEditor(LAUNCH_ARGS);
void launchRuntime(LAUNCH_ARGS);

int main(int argc, char *argv[]) {
  bool editor = true; // We use the editor by default

  for (int i = 0; i < argc; i++) {
    if (strcmp(argv[i], "--runtime") == 0) {
      editor = false;
    }
  }

  GLFWwindow *window = Jenjin::Helpers::CreateWindow(800, 600, "Jenjin");
  Jenjin::Helpers::CheckWindow(window);
  Jenjin::Helpers::InitiateImGui(window);

  Jenjin::Engine engine(window);
  auto scene = std::make_shared<Jenjin::Scene>();
  engine.AddScene(scene, true);

	spdlog::info("Launching Jenjin {}", editor ? "Editor" : "Runtime");
  (editor ? launchEditor : launchRuntime)(engine, window, scene);
}

void launchEditor(LAUNCH_ARGS) {
  Jenjin::Editor::ensure_dir(Jenjin::Editor::get_jendir() + "/Projects");

  auto editor = Jenjin::Targets::EditorTarget();
  scene->SetTarget(&editor);

  while (!glfwWindowShouldClose(window)) {
    engine.Render(&editor);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
}

void launchRuntime(LAUNCH_ARGS) {
  auto runtime = Jenjin::Targets::RuntimeTarget();
  scene->SetTarget(&runtime);

	if (!std::filesystem::exists("scripts")) {
		spdlog::error("Could not find `scripts` directory");
		exit(1);
	}

	scene->GetLuaManager()->LoadDirectory("scripts");

  // Look for `main.jenscene` in the current directory
  if (!std::filesystem::exists("main.jenscene")) {
    spdlog::error("Could not find `main.jenscene` in the current directory");
    exit(1);
  }

  scene->Load("main.jenscene");
	scene->GetLuaManager()->Ready();

  while (!glfwWindowShouldClose(window)) {
    engine.Render(&runtime);

    glfwPollEvents();
    glfwSwapBuffers(window);
  }
}
