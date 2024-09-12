#include "scene.h"
#include "gameobject.h"
#include "engine.h"
#include <memory>

int main(void) {
	auto object1 = std::make_shared<Jenjin::GameObject>("Test1");
	auto object2 = std::make_shared<Jenjin::GameObject>("Test2");

	object1->set_texture("test/assets/container.jpg");

	object1->set_position(glm::vec2(-1, -1));
	object1->set_rotation(45.0);

	object2->set_position(glm::vec2(1, 1));
	object2->set_rotation(45.0);
	object2->set_scale(glm::vec2(0.5f, 0.5f));

	object1->set_color(glm::vec3(1.0f, 0.0f, 0.0f));
	object2->set_color(glm::vec3(0.0f, 1.0f, 0.0f));

	auto scene = new Jenjin::Scene({ object1, object2 });
	Jenjin::Engine.add_scene(scene, true);
	Jenjin::Engine.launch(600, 600, "Test");
}
