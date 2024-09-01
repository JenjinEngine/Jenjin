#include "engine.h"
#include "gameobject.h"
#include "scene.h"

#include "shapes.h"
#include "state.h"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

int score = 0;

void update_ball(Jenjin::GameObject& ball, glm::vec3& velocity, Jenjin::GameObject& left_paddle, Jenjin::GameObject& right_paddle) {
	static float time_since_ball_action = 0.0f;

	time_since_ball_action += JenjinState.deltaTime;

	if (time_since_ball_action > 0.05f) {
		if (ball.transform.position.x >= 1.0f - 0.025 || ball.transform.position.x <= -1.0f + 0.025) {
			velocity.x *= -1.0f;
			time_since_ball_action = 0.0f;

			if (ball.transform.position.x >= 1.0f - 0.025) {
				score++;
				spdlog::info("Score: {}", score);
			} else {
				score--;
				spdlog::info("Score: {}", score);
			}
		}

		if (ball.transform.position.y >= 1.0f - 0.025 || ball.transform.position.y <= -1.0f + 0.025) {
			velocity.y *= -1.0f;
			time_since_ball_action = 0.0f;
		}

		if ((ball.transform.position.x <= -0.9f + 0.025 && ball.transform.position.y <= left_paddle.transform.position.y + 0.15f && ball.transform.position.y >= left_paddle.transform.position.y - 0.15f) ||
			(ball.transform.position.x >= 0.9f - 0.025 && ball.transform.position.y <= right_paddle.transform.position.y + 0.15f && ball.transform.position.y >= right_paddle.transform.position.y - 0.15f)) {
			velocity.x *= -1.0f;
			time_since_ball_action = 0.0f;
		}
	}

	ball.translate(velocity * JenjinState.deltaTime);
}

void update_paddle(Jenjin::GameObject& paddle, GLFWwindow* window) {
	paddle.set_rotation(0.0f);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		paddle.set_rotation(2.0f);
		paddle.translate(glm::vec3(0.0f, 0.5f * JenjinState.deltaTime, 0.0f));
	}

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		paddle.set_rotation(-2.0f);
		paddle.translate(glm::vec3(0.0f, -0.5f * JenjinState.deltaTime, 0.0f));
	}
}

void update_ai(Jenjin::GameObject& paddle, Jenjin::GameObject& ball) {
	if (ball.transform.position.y > paddle.transform.position.y) {
		paddle.translate(glm::vec3(0.0f, 0.5f * JenjinState.deltaTime, 0.0f));
	}
	if (ball.transform.position.y < paddle.transform.position.y) {
		paddle.translate(glm::vec3(0.0f, -0.5f * JenjinState.deltaTime, 0.0f));
	}
}

int main(void) {
	Jenjin::Engine engine;

	// Create paddle objects
	Jenjin::GameObject leftPaddle = Jenjin::shapes::create_quad(0.05f, 0.3f);
	Jenjin::GameObject rightPaddle = Jenjin::shapes::create_quad(0.05f, 0.3f);
	leftPaddle.set_position(glm::vec3(-0.9f, 0.0f, 0.0f));
	rightPaddle.set_position(glm::vec3(0.9f, 0.0f, 0.0f));

	// Create ball object
	Jenjin::GameObject ball = Jenjin::shapes::create_quad(0.05f, 0.05f);

	// Create a scene with the meshes
	Jenjin::Scene scene({ &leftPaddle, &rightPaddle, &ball });

	// Add scene to engine
	engine.add_scene(scene);
	engine.activate_scene(0);

	glm::vec3 ball_velocity = glm::vec3(0.6f, -0.8f, 0.0f);

	engine.set_render_callback([&](Jenjin::Engine* engine, GLFWwindow* window) {
		update_ball(ball, ball_velocity, leftPaddle, rightPaddle);
		update_paddle(leftPaddle, window);
		update_ai(rightPaddle, ball);
	});

	// Launch engine
	engine.launch(800, 800, "Pong");
}

