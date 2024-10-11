#pragma once

#include "jenjin/luamanager.h"
#include "jenjin/scene.h"
#include "jenjin/target.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Jenjin {
class Engine {
public:
  Engine(GLFWwindow *window);
  ~Engine();

  void AddScene(std::shared_ptr<Scene> scene, bool defaultScene = false);

  void Render(Target *target); // Render directly to the target

  GLFWwindow *glfwContext;

  Scene *GetCurrentScene() { return currentScene; }

private:
  std::vector<std::shared_ptr<Scene>> scenes = {};
  Scene *currentScene = nullptr;

	LuaManager luaManager;
};

extern Engine *EngineRef;
} // namespace Jenjin
