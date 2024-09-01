#ifndef CAMERA_H
#define CAMERA_H

#include "shader.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Jenjin {
class Camera {
private:
	glm::vec3 m_cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
	glm::mat4 m_view = glm::lookAt(m_cameraPos, m_cameraPos + glm::vec3(0, 0, -1), glm::vec3(0.0f, 1.0f, 0.0f));

	float m_zoom = 1.0f;

	// TODO: Have delta time calculated as part of the engine
	float m_deltaTime = 0.0f;
	float m_lastFrame = 0.0f;

	bool m_changed_projection = false;

public:
	Camera();

	const float cameraSpeed;
	const glm::vec3 cameraUp;
	const glm::vec3 cameraRight;
	const glm::vec3 cameraFront;

	void processInput(GLFWwindow* window);
	void update_deltas();
	void bind_uniforms(Shader& shader);
	void setup_proj(Shader& shader);

	bool has_new_projection();
};
}

#endif // CAMERA_H
