#ifndef JENJIN_ENGINE_H
#define JENJIN_ENGINE_H

#include "GLFW/glfw3.h"
namespace Jenjin {
class Engine {
public:
	Engine();
	~Engine() = default;

	void Launch(int width, int height, const char* title);

private:
	GLFWwindow* window;
};
}

#endif // JENJIN_ENGINE_H
