#include "scene.h"
#include "engine.h"

#include <fstream>

int main(void) {
	std::ifstream file("test.jenscene");
	auto scene = new Jenjin::Scene();
	scene->load(file);

	Jenjin::Engine->add_scene(scene, true);
	Jenjin::Engine->launch(800, 600, "Test");
}
