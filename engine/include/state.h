#ifndef JENJIN_STATE_H
#define JENJIN_STATE_H

#include "engine.h"
#include "lua.h"
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
} extern JenjinState;

#endif // JENJIN_STATE_H
