#pragma once

#include "jenjin/target.h"

namespace Jenjin {
namespace Targets {
class DefaultTarget : public Jenjin::Target {
public:
  DefaultTarget() = default;

  virtual void PreRender() override;

  virtual glm::vec2 GetSize() override;
  virtual void Resize(glm::vec2 size) override;

  virtual glm::vec2 GetMousePosition() override;
};
} // namespace Targets
} // namespace Jenjin
