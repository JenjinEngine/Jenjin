#ifndef JENJIN_ENGINE_H
#define JENJIN_ENGINE_H

#include "scene.h"

#include <GLFW/glfw3.h>

namespace Jenjin {
// The main engine class that manages the game loop
// and scene management.
class Engine_t {
public:
	// RAII pattern
	Engine_t();
	~Engine_t() = default;

	// Scene management
	void add_scene(Scene* scene, bool active = false);
	void activate_scene(unsigned int index);

	// Launch the engine
	void launch(int width, int height, const char* title);

private:
	GLFWwindow* window;

	std::vector<Scene*> m_scenes;
	Scene* m_active_scene;
} extern Engine;
}

#endif // JENJIN_ENGINE_H
