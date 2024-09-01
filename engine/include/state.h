#ifndef JENJIN_STATE_H
#define JENJIN_STATE_H

#include "engine.h"
#include "window.h"

using namespace Jenjin;

struct State {
	float deltaTime = 0.0f;
	Window* window = nullptr;
	Engine* engine = nullptr;
} extern JenjinState;

#endif // JENJIN_STATE_H
