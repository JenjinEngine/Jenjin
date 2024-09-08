#ifndef CAMERA_H
#define CAMERA_H

#include "shader.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Jenjin {
class Camera {
private:
	glm::mat4 m_view = glm::lookAt(position, position + cameraFront, cameraUp);

public:
	glm::vec3 position = glm::vec3(0.0f, 0.0f, 100.0f);
	float zoom = 1.0f;

	Camera();

	const float cameraSpeed;
	const glm::vec3 cameraUp;
	const glm::vec3 cameraRight;
	const glm::vec3 cameraFront;

	void processInput(GLFWwindow* window);
	void bind_uniforms(Shader& shader);
	void setup_proj(Shader& shader);

	bool m_changed_projection = false;
};
}

#endif // CAMERA_H
