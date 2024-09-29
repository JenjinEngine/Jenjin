#include "jenjin/helpers.h"
#include "jenjin/engine.h"
#include "jenjin/scene.h"

#include "mydisplay.h"

#include <spdlog/spdlog.h>

int main(void) {
	GLFWwindow* window = Jenjin::Helpers::CreateWindow(800, 600, "Test");
	Jenjin::Helpers::CheckWindow(window);
	Jenjin::Helpers::InitiateImGui(window);

	Jenjin::Engine engine(window);
	MyDisplay myDisplay;

	auto scene = std::make_shared<Jenjin::Scene>();

	auto gameObject1 = std::make_shared<Jenjin::GameObject>("1", Jenjin::Helpers::CreateQuad(2, 2));
	auto gameObject2 = std::make_shared<Jenjin::GameObject>("2", Jenjin::Helpers::CreateQuad(2, 2));
	auto gameObject3 = std::make_shared<Jenjin::GameObject>("3", Jenjin::Helpers::CreateQuad(2, 2));
	gameObject1->transform.position = glm::vec3(-2, 0, 0);
	gameObject3->transform.position = glm::vec3( 2, 0, 0);

	scene->AddGameObject(gameObject1);
	scene->AddGameObject(gameObject2);
	scene->AddGameObject(gameObject3);

	scene->Build();

	scene->SetTarget(&myDisplay);

	engine.AddScene(scene, true);

	while (!glfwWindowShouldClose(window)) {
		engine.Render(&myDisplay);

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
