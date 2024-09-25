#pragma once

#include "imgui.h"
#include "shader.h"

#include <gtc/matrix_transform.hpp>
#include <glm.hpp>

#include <GLFW/glfw3.h>

namespace Jenjin {
// A 2D camera that can be moved around and zoomed
// in and out.
class Camera {
private:
	// View matrix
	glm::mat4 m_view = glm::lookAt(position, position + cameraFront, cameraUp);

public:
	Camera();

	// Camera vectors
	const glm::vec3 cameraRight;
	const glm::vec3 cameraFront;
	const glm::vec3 cameraUp;
	const float cameraSpeed;

	// Functions for camera movement
	void processInput(GLFWwindow* window);
	void bind_uniforms(Shader& shader);
	void setup_proj(Shader& shader);

	glm::vec3 position = glm::vec3(0.0f, 0.0f, 100.0f);
	glm::vec3 clearColor = glm::vec3(ImGui::GetStyle().Colors[ImGuiCol_WindowBg].x, ImGui::GetStyle().Colors[ImGuiCol_WindowBg].y, ImGui::GetStyle().Colors[ImGuiCol_WindowBg].z);

	float aspect_ratio = 1.0f;
	float rotation = 0.0f;
	float zoom = 1.0f;

	// Getters
	glm::vec3 get_position() { return this->position; };
	glm::vec3 get_clear_color() { return this->clearColor; };
	float get_zoom() { return this->zoom; };
	float get_rotation() { return this->rotation; };
	float get_aspect_ratio() { return this->aspect_ratio; };

	// Setters
	void set_position(const glm::vec3& position) { this->position = position; };
	void set_clear_color(const glm::vec3& clearColor) { this->clearColor = clearColor; };
	void set_zoom(float zoom) { this->zoom = zoom; };
	void set_rotation(float rotation) { this->rotation = rotation; };
	void set_aspect_ratio(float aspect_ratio) { this->aspect_ratio = aspect_ratio; };
};
}
