#ifndef JENJIN_ENGINE_H
#define JENJIN_ENGINE_H

#include "framebuffer.h"
#ifdef JENJIN_HEADLESS
#include "scene.h"
#endif

#include "scene.h"

#include <GLFW/glfw3.h>

#define JENJIN_VERSION "0.0.1"

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

	// The active scene pointer
	Scene* active_scene;

	// Engine state
	bool running = false;

	void set_window(GLFWwindow* window) {
		this->window = window;
	}

#ifdef JENJIN_HEADLESS
	Framebuffer framebuffer;
	void render_into_imgui(int width, int height);
#endif

private:
	GLFWwindow* window;

	std::vector<Scene*> m_scenes;

#ifdef JENJIN_HEADLESS
	int ow, oh;
#endif
} extern* Engine;
}

#endif // JENJIN_ENGINE_H
