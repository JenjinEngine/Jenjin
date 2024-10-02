#define JENJIN_EDITOR
/* #define JENJIN_RUNTIME */

#ifdef JENJIN_EDITOR
#include "jenjin/targets/editor.h"
#else
#include "jenjin/targets/default.h"
#endif

#include "jenjin/helpers.h"
#include "jenjin/engine.h"
#include "jenjin/scene.h"

#include <spdlog/spdlog.h>

#include <cstdlib>
#include <memory>


int main(void) {
#ifdef JENJIN_EDITOR
#ifdef JENJIN_RUNTIME
	spdlog::error("Cannot define both JENJIN_EDITOR and JENJIN_RUNTIME");
	exit(EXIT_FAILURE);
#endif
#endif

	GLFWwindow* window = Jenjin::Helpers::CreateWindow(800, 600, "Test");
	Jenjin::Helpers::CheckWindow(window);
	Jenjin::Helpers::InitiateImGui(window);

	Jenjin::Engine engine(window);

	auto scene = std::make_shared<Jenjin::Scene>();

	scene->Build();
	engine.AddScene(scene, true);

#ifdef JENJIN_EDITOR
	Jenjin::Targets::EditorTarget editorTarget;
	scene->SetTarget(&editorTarget);
#else
	Jenjin::Targets::DefaultTarget defaultTarget;
	scene->SetTarget(&defaultTarget);
#endif

	while (!glfwWindowShouldClose(window)) {
#ifdef JENJIN_EDITOR
		engine.Render(&editorTarget);
#else
		engine.Render(&defaultTarget);
#endif

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}
