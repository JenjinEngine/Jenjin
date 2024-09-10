#include "jenjin/engine.h"
#include "jenjin/helpers.h"

#include "display.h"

#include <spdlog/spdlog.h>

int main(void) {
	GLFWwindow* window = Jenjin::Helpers::CreateWindow(800, 600, "Test");
	Jenjin::Helpers::CheckWindow(window);
	Jenjin::Helpers::InitiateImGui(window);

	Jenjin::Engine engine(window);

	Display display;

	while (!glfwWindowShouldClose(window)) {
		engine.Render(&display);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
