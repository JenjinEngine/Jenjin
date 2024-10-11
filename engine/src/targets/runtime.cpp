#define GLFW_INCLUDE_NONE

#include "jenjin/targets/runtime.h"
#include "jenjin/engine.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

using namespace Jenjin::Targets;

void RuntimeTarget::PreRender() {
  auto scene = Jenjin::EngineRef->GetCurrentScene();
  scene->Update();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  // HACK: This is a hack to set the camera position to 0,0,0..
  // This needs to be done properly.
  static bool done = false;
  if (!done) {
    done = true;
    scene->GetCamera()->SetPosition(glm::vec3(0, 0, 0));
  }
}

glm::vec2 RuntimeTarget::GetSize() {
  int width, height;
  static auto ctx = glfwGetCurrentContext();
  glfwGetWindowSize(ctx, &width, &height);

  return glm::vec2(width, height);
}

void RuntimeTarget::Resize(glm::vec2 size) {
  static auto ctx = glfwGetCurrentContext();
  Jenjin::EngineRef->GetCurrentScene()->GetCamera()->Resize(size);
  glfwSetWindowSize(ctx, (int)size.x, (int)size.y);
  glViewport(0, 0, (int)size.x, (int)size.y);
}

glm::vec2 RuntimeTarget::GetMousePosition() {
  static auto ctx = glfwGetCurrentContext();
  static double x, y;
  glfwGetCursorPos(ctx, &x, &y);
  return glm::vec2(x, y);
}
