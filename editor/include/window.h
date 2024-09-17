#include "scene.h"
#include <functional>

#include <GLFW/glfw3.h>

class Window {
public:
	Window();

	void render(std::function<void (Jenjin::Scene*)> render_callback);
	void launch(int width, int height);

	GLFWwindow* context;

private:
	std::function<void (Jenjin::Scene*)> m_render_callback;
};
