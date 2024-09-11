#include "scene.h"
#include "gameobject.h"
#include "engine.h"
#include <memory>

int main(void) {
	auto object1 = std::make_shared<Jenjin::GameObject>("Test1");
	auto object2 = std::make_shared<Jenjin::GameObject>("Test2");

	object1->transform.position = glm::vec2(-0.05f, -0.05f);
	object2->transform.position = glm::vec2(0.05f, 0.05f);

	auto scene = new Jenjin::Scene({ object1, object2 });
	Jenjin::Engine.add_scene(scene, true);
	Jenjin::Engine.launch(600, 600, "Test");
}
