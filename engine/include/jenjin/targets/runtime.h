#pragma once

#include "jenjin/target.h"

namespace Jenjin {
namespace Targets {
class RuntimeTarget : public Jenjin::Target {
public:
  RuntimeTarget() = default;

  virtual void PreRender() override;

  virtual glm::vec2 GetSize() override;
  virtual void Resize(glm::vec2 size) override;

  virtual glm::vec2 GetMousePosition() override;
};
} // namespace Targets
} // namespace Jenjin
