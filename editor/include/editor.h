#pragma once

#include "gameobject.h"
#include "imgui.h"
#include "scene.h"

namespace JenjinEditor {
enum RenderableWindow {
	Scene,
	Viewport
};

class Editor {
public:
	Editor();

	void menu();
	void dockspace();

	void controls(Jenjin::Scene* scene);
	void hierarchy(Jenjin::Scene* scene);
	void inspector(Jenjin::Scene* scene);
	void scene(Jenjin::Scene* scene);
	void viewport(Jenjin::Scene* scene);
	void explorer(Jenjin::Scene* scene);
	void code(Jenjin::Scene* scene);

	void backup_prompts(Jenjin::Scene* scene);

	void show_all(Jenjin::Scene* scene);

private:
	Jenjin::GameObject* selectedObject = nullptr;
	bool selectedCamera = false;

	RenderableWindow renderableWindow = Scene;
	ImVec2 renderableWindowSize = ImVec2(-1, -1);

	char renameGameObjectBuffer[256] = { 0 };
};
}
