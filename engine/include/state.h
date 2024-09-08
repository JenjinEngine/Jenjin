#ifndef JENJIN_STATE_H
#define JENJIN_STATE_H

#include "camera.h"
#include "engine.h"
#include "scene.h"
#include "scriptmanager.h"
#include "window.h"

using namespace Jenjin;

struct State {
	float dt = 0.0f;
	Window* window = nullptr;
	Engine* engine = nullptr;
	ScriptManager* script_manager = nullptr;
	Scene* scene = nullptr;
	Camera* camera = nullptr;

	glm::vec2 window_size = glm::vec2(0.0f, 0.0f);
	glm::vec2 mouse_pos = glm::vec2(0.0f, 0.0f);
} extern JenjinState;

#endif // JENJIN_STATE_H
