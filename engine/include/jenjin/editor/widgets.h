#pragma once

#include "jenjin/gameobject.h"
#include "jenjin/camera.h"

namespace Jenjin::Editor::Widgets {
void transformWidget(
    Jenjin::GameObject::Transform
        *transform);

void cameraWidget(Jenjin::Camera *camera);
};
