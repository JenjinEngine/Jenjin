#pragma once

#include "jenjin/gameobject.h"
namespace Jenjin::Editor::Widgets {
bool transformWidget(
    Jenjin::GameObject::Transform
        *transform); // Returns true if the transform was changed
};
