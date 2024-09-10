#include "scene.h"
#include "gameobject.h"
#include "engine.h"
#include <memory>

int main(void) {
	Jenjin::Engine engine;

	auto object1_ptr = std::make_shared<Jenjin::GameObject>("Test1");
	auto object2_ptr = std::make_shared<Jenjin::GameObject>("Test2");

	object1_ptr->transform.position = glm::vec2(-0.05f, -0.05f);
	object2_ptr->transform.position = glm::vec2(0.05f, 0.05f);

	auto scene = new Jenjin::Scene({ object1_ptr, object2_ptr });
	engine.add_scene(scene, true);

	engine.launch(600, 600, "Test");
}
