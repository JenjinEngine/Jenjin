#include "jenjin/engine.h"
#include "jenjin/helpers.h"

#include "jenjin/scene.h"
#include "mydisplay.h"

#include <spdlog/spdlog.h>

int main(void) {
	GLFWwindow* window = Jenjin::Helpers::CreateWindow(800, 600, "Test");
	Jenjin::Helpers::CheckWindow(window);
	Jenjin::Helpers::InitiateImGui(window);

	Jenjin::Engine engine(window);

	auto scene = std::make_shared<Jenjin::Scene>();
	auto gameObject = std::make_shared<Jenjin::GameObject>(Jenjin::Helpers::CreateQuad(2, 2));
	scene->AddGameObject(gameObject);
	scene->Build();

	engine.AddScene(scene, true);

	MyDisplay mydisplay;

	while (!glfwWindowShouldClose(window)) {
		engine.Render(&mydisplay);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
