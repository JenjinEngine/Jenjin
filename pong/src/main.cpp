#include "engine.h"
#include "gameobject.h"
#include "scene.h"
#include "shapes.h"
#include "state.h"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <vector>

std::vector<GameObject*> to_clean = {};

void main_menu_scene_update(Jenjin::Scene* scene) {
	if (glfwGetKey(JenjinState.window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
		JenjinState.engine->activate_scene(1);
}

Scene* create_main_menu_scene(std::unique_ptr<Engine>& engine) {
	GameObject* placeholder = new GameObject(Jenjin::shapes::create_quad(0.5f, 0.5f), "placeholder");
	Scene* scene = new Scene({ placeholder });

	scene->set_update_callback(main_menu_scene_update);
	to_clean.insert(to_clean.end(), { placeholder });

	engine->add_scene(scene);

	return scene;
}

void game_scene_update(Jenjin::Scene* scene) {
	JenjinState.script_manager->update();
}

Scene* create_game_scene(std::unique_ptr<Engine>& engine) {
	// Create paddle objects
	Jenjin::GameObject* leftPaddle = new GameObject(Jenjin::shapes::create_quad(0.05f, 0.3f), "left_paddle");
	Jenjin::GameObject* rightPaddle = new GameObject(Jenjin::shapes::create_quad(0.05f, 0.3f), "right_paddle");
	leftPaddle->set_position(glm::vec2(-0.9f, 0.0f));
	rightPaddle->set_position(glm::vec2(0.9f, 0.0f));

	// Create ball object
	Jenjin::GameObject* ball = new GameObject(Jenjin::shapes::create_quad(0.05f, 0.05f), "ball");

	// Create a scene with the meshes
	Scene* scene = new Scene({ leftPaddle, rightPaddle, ball });
	to_clean.insert(to_clean.end(), { leftPaddle, rightPaddle, ball });

	// Add scene to engine
	engine->add_scene(scene);

	scene->set_update_callback(game_scene_update);

	return scene;
}

int main(void) {
	std::unique_ptr<Engine> engine = std::make_unique<Engine>();
	JenjinState.script_manager->add_directory("pong/scripts");

	Scene* main_menu_scene = create_main_menu_scene(engine);
	Scene* game_scene = create_game_scene(engine);

	engine->activate_scene(0);

	// Launch engine
	engine->launch(800, 800, "Pong");
}

