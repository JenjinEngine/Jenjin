#pragma once

#include "jenjin/editor/editor.h"
#include "jenjin/framebuffer.h"
#include "jenjin/target.h"

namespace Jenjin {
namespace Targets {
class EditorTarget : public Jenjin::Target {
public:
  EditorTarget();

  virtual void PreRender() override;
  virtual void Render() override;
  virtual void PostRender() override;

  virtual glm::vec2 GetSize() override;
  virtual void Resize(glm::vec2 size) override;

  virtual glm::vec2 GetMousePosition() override;

  virtual bool RespondsToWindowResize() override;

  Jenjin::Framebuffer renderTexture;
  int width, height;

  Jenjin::Editor::Manager editor;
};
} // namespace Targets
} // namespace Jenjin
