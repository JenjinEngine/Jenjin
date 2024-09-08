#include "engine.h"
#include "gameobject.h"
#include "scene.h"
#include "shapes.h"
#include "state.h"

#include <GLFW/glfw3.h>

using namespace Jenjin;

int main(void) {
	std::unique_ptr<Engine> engine = std::make_unique<Engine>();

	GameObject* player = new GameObject(shapes::create_quad(0.1f, 0.1f), "player");
	GameObject* mouse = new GameObject(shapes::create_quad(0.025f, 0.025f), "mouse");
	Scene* scene = new Scene({ player, mouse });

	engine->add_scene(scene);
	engine->activate_scene(0);

	scene->set_update_callback([](Scene* scene) {
		JenjinState.script_manager->update();
	});

	JenjinState.script_manager->add_directory("wave_defense/scripts");

	engine->launch(800, 800, "Wave Defense");
}
