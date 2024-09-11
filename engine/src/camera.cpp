#include "camera.h"

#include <spdlog/spdlog.h>

using namespace Jenjin;

Camera::Camera() : cameraSpeed(2.5f), cameraUp(0.0f, 1.0f, 0.0f), cameraRight(1.0f, 0.0f, 0.0f), cameraFront(0.0f, 0.0f, -1.0f) {}

void Camera::processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
		return;

	const float cameraSpeed = 0.05;

	if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		this->zoom += cameraSpeed;
	} else if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		this->zoom -= cameraSpeed;

		if (this->zoom <= 0.05f)
			this->zoom = 0.05f;
	}

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		this->position -= cameraRight * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		this->position += cameraRight * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		this->position += cameraSpeed * cameraUp;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		this->position -= cameraSpeed * cameraUp;
}

void Camera::bind_uniforms(Shader& shader) {
	m_view = glm::lookAt(this->position, this->position + cameraFront, cameraUp);
	shader.set("u_view", m_view);
}

void Camera::setup_proj(Shader& shader) {
	glm::mat4 projection = glm::ortho(-1.0f * this->zoom, 1.0f * this->zoom, -1.0f * this->zoom, 1.0f * this->zoom, 0.1f, 100.0f);
	projection = glm::scale(projection, glm::vec3(1.0f, aspect_ratio, 1.0f));
	shader.set("u_projection", projection);
}
