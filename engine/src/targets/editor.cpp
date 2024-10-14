#define GLFW_INCLUDE_NONE

#include "jenjin/targets/editor.h"
#include "jenjin/engine.h"
#include "jenjin/framebuffer.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <IconsFontAwesome6.h>
#include <glm/gtc/type_ptr.hpp>

#include <GLFW/glfw3.h>

#define VIEWPORT_TITLE ICON_FA_VIDEO " Viewport";

using namespace Jenjin::Targets;

EditorTarget::EditorTarget() {}

void EditorTarget::PreRender() {
  // HACK: This is a hack to set the camera position to 0,0,0..
  // This needs to be done properly.
  static bool done = false;
  if (!done) {
    done = true;
    Jenjin::EngineRef->GetCurrentScene()->GetCamera()->SetPosition(
        glm::vec3(0, 0, 0));
  }

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);

  Resize(GetSize());

  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  this->editor.show_all(Jenjin::EngineRef->GetCurrentScene());

  renderTexture.Bind();

  glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT);
  glViewport(0, 0, width, height);
}

void EditorTarget::Render() {
  renderTexture.Unbind();

  if (!this->editor.hasProjectOpen) {
    return;
  }

  static auto title = VIEWPORT_TITLE;
  ImGui::Begin(title, nullptr,
               ImGuiWindowFlags_NoScrollbar |
                   ImGuiWindowFlags_NoScrollWithMouse);

  ImVec2 size = ImGui::GetContentRegionAvail();
  this->width = size.x;
  this->height = size.y;

  ImGui::GetWindowDrawList()->AddImage(
      (void *)(intptr_t)renderTexture.texture,
      ImVec2(ImGui::GetCursorScreenPos()),
      ImVec2(ImGui::GetCursorScreenPos().x + size.x,
             ImGui::GetCursorScreenPos().y + size.y),
      ImVec2(0, 1), ImVec2(1, 0));

  std::string res_text =
      std::to_string((int)size.x) + "x" + std::to_string((int)size.y);
  ImGui::GetWindowDrawList()->AddText(
      ImVec2(ImGui::GetCursorScreenPos().x + 10,
             ImGui::GetCursorScreenPos().y + 10),
      ImColor(255, 255, 255, 255), res_text.c_str());

  ImGui::End();
}

void EditorTarget::PostRender() {
  ImGui::EndFrame();
  ImGui::Render();

  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

glm::vec2 EditorTarget::GetSize() { return glm::vec2(width, height); }

void EditorTarget::Resize(glm::vec2 size) {
  this->width = size.x;
  this->height = size.y;
  renderTexture.Resize(width, height);
  Jenjin::EngineRef->GetCurrentScene()->GetCamera()->Resize(size);
}

glm::vec2 EditorTarget::GetMousePosition() {
  static auto ctx = glfwGetCurrentContext();
  static double gx, gy;
  glfwGetCursorPos(ctx, &gx, &gy);

  auto v = ImGui::GetWindowPos(); // HACK: This is wrong!!! It doesn't get the
                                  // viewport window pos.
  auto lx = gx - v.x;
  auto ly = gy - v.y;

  return glm::vec2(lx, ly);
}

// We handle window resizing ourselves
bool EditorTarget::RespondsToWindowResize() { return false; }
