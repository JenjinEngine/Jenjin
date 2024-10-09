#pragma once

#include <glm/glm.hpp>

namespace Jenjin {
// A "Target" is somewhere for the engine to render to. This class is completely
// virtual to be overriden by something else
//
// For example, a general target could be just a simple output however another
// may be a framebuffer for inside an editor using ImGui to display the output
// like a viewport.
class Target {
public:
  virtual void PreRender() {};  // Called before rendering
  virtual void Render() {};     // Called during rendering (usually not needed)
  virtual void PostRender() {}; // Called after rendering

  // Get the size of the target
  virtual glm::vec2 GetSize() { return glm::vec2(-1, -1); };

  // Resize the target
  virtual void Resize(glm::vec2 size) {};

  // Get the mouse position
  virtual glm::vec2 GetMousePosition() { return glm::vec2(-1, -1); };

  // Does the target respond to events of the window resizing
  virtual bool RespondsToWindowResize() { return true; };
};
} // namespace Jenjin
