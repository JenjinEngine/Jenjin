#ifndef JENJIN_ENGINE_H
#define JENJIN_ENGINE_H

#include "scene.h"
#include "shader.h"
#include "window.h"
#include <vector>

namespace Jenjin {
class Engine {
public:
	Engine();
	~Engine() = default;

	void check_version(void);

	void launch(int width, int height, const char* title);

	void add_scene(const Scene& scene);
	bool activate_scene(int index);

	void set_resized_callback(GLFWwindowsizefun callback);
	void set_key_callback(GLFWkeyfun callback);
	void set_mouse_callback(GLFWcursorposfun callback);

private:
	Window m_window;
	Scene* m_active_scene = nullptr;
	std::vector<Scene> m_scenes;
};
}

#endif // JENJIN_ENGINE_H
