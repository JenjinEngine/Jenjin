#include "engine.h"
#include "gameobject.h"
#include "scene.h"
#include "shapes.h"
#include "state.h"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <vector>

int score = 0;
const float PAD_SPEED = 0.65f;
const glm::vec3 DEFAULT_BALL_SPEED = glm::vec3(0.6f, 0.8f, 0.0f);

std::vector<GameObject*> to_clean = {};

static bool playing = false;

void update_ball(Jenjin::GameObject* ball, glm::vec3* velocity, Jenjin::GameObject* left_paddle, Jenjin::GameObject* right_paddle, GLFWwindow* window) {
	if (!playing) {
		if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
			playing = true;
		}

		return;
	}

	// Rotation effect
	if (velocity->x > 0) {
		ball->rotate(180.0f * JenjinState.deltaTime);
	} else {
		ball->rotate(-180.0f * JenjinState.deltaTime);
	}

	// Check top and bottom bounds
	if (ball->transform.position.y > 1.0f - 0.025f) {
		velocity->y = -velocity->y;
		ball->transform.position.y = 1.0f - 0.025f;
	} else if (ball->transform.position.y < -1.0f + 0.025f) {
		velocity->y = -velocity->y;
		ball->transform.position.y = -1.0f + 0.025f;
	}

	// Bounce on paddles (invert x velocity)
	// Left paddle
	if (ball->transform.position.x < left_paddle->transform.position.x + 0.025f &&
		ball->transform.position.y < left_paddle->transform.position.y + 0.15f &&
		ball->transform.position.y > left_paddle->transform.position.y - 0.15f) {
		ball->transform.position.x += 0.025f;
		velocity->x = -velocity->x;
	}

	// Right paddle
	if (ball->transform.position.x > right_paddle->transform.position.x - 0.025f &&
		ball->transform.position.y < right_paddle->transform.position.y + 0.15f &&
		ball->transform.position.y > right_paddle->transform.position.y - 0.15f) {
		ball->transform.position.x -= 0.025f;
		velocity->x = -velocity->x;
	}

	// Check left and right bounds
	if (ball->transform.position.x > 1.0f || ball->transform.position.x < -1.0f) {
		if (ball->transform.position.x > 1.0f) {
			score++;
			spdlog::info("Score: {}", score);
		}
		else {
			spdlog::info("Game Over!");
			score = 0;
			playing = false;
		}

		ball->set_position(glm::vec3(0.0f, 0.0f, 0.0f));
		ball->set_rotation(0.0f);

		left_paddle->set_position(glm::vec3(-0.9f, 0.0f, 0.0f));
		right_paddle->set_position(glm::vec3(0.9f, 0.0f, 0.0f));

		*velocity = DEFAULT_BALL_SPEED;
	}

	// Move ball
	ball->translate(*velocity * JenjinState.deltaTime);
}

void update_paddle(Jenjin::GameObject* paddle, GLFWwindow* window) {
	paddle->set_rotation(0.0f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		paddle->set_rotation(-2.0f);
		paddle->translate(glm::vec3(0.0f, PAD_SPEED * JenjinState.deltaTime, 0.0f));
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		paddle->set_rotation(2.0f);
		paddle->translate(glm::vec3(0.0f, -PAD_SPEED * JenjinState.deltaTime, 0.0f));
	}
}

void update_ai(Jenjin::GameObject* paddle, Jenjin::GameObject* ball) {
	paddle->set_rotation(0.0f);

	if (ball->transform.position.y > paddle->transform.position.y) {
		paddle->set_rotation(2.0f);
		paddle->translate(glm::vec3(0.0f, PAD_SPEED * JenjinState.deltaTime, 0.0f));
	}

	if (ball->transform.position.y < paddle->transform.position.y) {
		paddle->set_rotation(-2.0f);
		paddle->translate(glm::vec3(0.0f, -PAD_SPEED * JenjinState.deltaTime, 0.0f));
	}
}

void main_menu_scene_update(Jenjin::Scene* scene) {
	if (glfwGetKey(JenjinState.window->getWindow(), GLFW_KEY_SPACE) == GLFW_PRESS)
		JenjinState.engine->activate_scene(1);
}

Scene* create_main_menu_scene(Engine* engine) {
	GameObject* placeholder = new GameObject(Jenjin::shapes::create_quad(0.5f, 0.5f), "placeholder");
	to_clean.push_back(placeholder);

	Scene* scene = new Scene({ placeholder });
	scene->set_update_callback(main_menu_scene_update);

	engine->add_scene(scene);

	return scene;
}

void game_scene_update(Jenjin::Scene* scene) {
	static GameObject* ball = scene->get_gameobject("ball");
	static GameObject* leftPaddle = scene->get_gameobject("left_paddle");
	static GameObject* rightPaddle = scene->get_gameobject("right_paddle");

	static glm::vec3 ball_velocity = DEFAULT_BALL_SPEED;

	update_ball(ball, &ball_velocity, leftPaddle, rightPaddle, JenjinState.window->getWindow());
	update_paddle(leftPaddle, JenjinState.window->getWindow());
	update_ai(rightPaddle, ball);
}

Scene* create_game_scene(Engine* engine) {
	// Create paddle objects
	Jenjin::GameObject* leftPaddle = new GameObject(Jenjin::shapes::create_quad(0.05f, 0.3f), "left_paddle");
	Jenjin::GameObject* rightPaddle = new GameObject(Jenjin::shapes::create_quad(0.05f, 0.3f), "right_paddle");
	leftPaddle->set_position(glm::vec3(-0.9f, 0.0f, 0.0f));
	rightPaddle->set_position(glm::vec3(0.9f, 0.0f, 0.0f));

	// Create ball object
	Jenjin::GameObject* ball = new GameObject(Jenjin::shapes::create_quad(0.05f, 0.05f), "ball");

	// Create a scene with the meshes
	Scene* scene = new Scene({ leftPaddle, rightPaddle, ball });
	// add all of them at once
	to_clean.insert(to_clean.end(), { leftPaddle, rightPaddle, ball });

	// Add scene to engine
	engine->add_scene(scene);

	scene->set_update_callback(game_scene_update);

	return scene;
}

int main(void) {
	Engine* engine = new Engine();

	Scene* main_menu_scene = create_main_menu_scene(engine);
	Scene* game_scene = create_game_scene(engine);
	engine->activate_scene(0);

	// Launch engine
	engine->launch(800, 800, "Pong");

	delete main_menu_scene;
	delete game_scene;
	delete engine;

	for (GameObject* go : to_clean) {
		delete go;
	}
}

