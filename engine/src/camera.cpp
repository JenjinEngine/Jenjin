#include "camera.h"

#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>

using namespace Jenjin;

Camera::Camera() : cameraSpeed(2.5f), cameraUp(0.0f, 1.0f, 0.0f), cameraRight(1.0f, 0.0f, 0.0f), cameraFront(0.0f, 0.0f, -1.0f) {}

void Camera::processInput(GLFWwindow* window) {
	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) != GLFW_PRESS)
		return;

	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_SUBTRACT) == GLFW_PRESS) {
		m_zoom += 0.05f;
		m_changed_projection = true;
	} else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS) {
		m_zoom -= 0.05f;

		if (m_zoom <= 0.05f)
			m_zoom = 0.05f;

		m_changed_projection = true;
	}

	const float cameraSpeed = 2.5 * m_deltaTime;

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		m_cameraPos -= cameraRight * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		m_cameraPos += cameraRight * cameraSpeed;

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		m_cameraPos += cameraSpeed * cameraUp;

	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		m_cameraPos -= cameraSpeed * cameraUp;
}

void Camera::update_deltas() {
	float currentFrame = glfwGetTime();
	m_deltaTime = currentFrame - m_lastFrame;
	m_lastFrame = currentFrame;
}

void Camera::bind_uniforms(Shader& shader) {
	m_view = glm::lookAt(m_cameraPos, m_cameraPos + cameraFront, cameraUp);
	shader.set("u_view", m_view);
}

void Camera::setup_proj(Shader& shader) {
	glm::mat4 projection = glm::ortho(-1.0f * m_zoom, 1.0f * m_zoom, -1.0f * m_zoom, 1.0f * m_zoom, 0.1f, 100.0f);
	shader.set("u_projection", projection);
}


bool Camera::has_new_projection() {
	if (m_changed_projection) {
		m_changed_projection = false;
		return true;
	}

	return false;
}
