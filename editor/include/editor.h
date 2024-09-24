#pragma once

#include "gameobject.h"
#include "scene.h"

namespace JenjinEditor {
class Editor {
public:
	Editor();

	void hierarchy(Jenjin::Scene* scene);
	void inspector(Jenjin::Scene* scene);
	void viewport(Jenjin::Scene* scene);

	void show_all(Jenjin::Scene* scene);

private:
	Jenjin::GameObject* selectedObject = nullptr;
};
}
