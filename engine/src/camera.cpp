#include "jenjin/camera.h"
#include "spdlog/spdlog.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Jenjin;

Camera::Camera(Shader* shader, glm::vec2 size) : shader(shader), aspectRatio((float)size.x / (float)size.y) {
	float& width = size.x; float& height = size.y;

	// Set the camera's position to the center of the screen
	position = glm::vec3(width / 2, height / 2, 0);
	rotation = 0;
	zoom = 100;

	// Set the camera's projection matrix
	Resize(size);
}

void Camera::Resize(glm::vec2 size) {
	if (ow == 0 && oh == 0) {
		ow = size.x;
		oh = size.y;
	}

	float& width = size.x; float& height = size.y;

	int deltaWidth = width - ow;
	int deltaHeight = height - oh;

	if (deltaWidth != 0 || deltaHeight != 0) {
		glm::vec2 delta = glm::vec2(deltaWidth, deltaHeight);
		position += glm::vec3(delta.x / 2, delta.y / 2, 0);

		ow = width;
		oh = height;
	}

	aspectRatio = (float)width / (float)height;
	projection = glm::ortho((float)width, 0.0f, (float)height, 0.0f, -1.0f, 1.0f);
}

void Camera::Update() {
	view = glm::translate(glm::mat4(1.0f), position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1)) * glm::scale(glm::mat4(1.0f), glm::vec3(zoom));
}

void Camera::SetPosition(const glm::vec3& position) {
	this->position = position;
}

void Camera::SetRotation(float rotation) {
	this->rotation = rotation;
}

void Camera::SetZoom(float zoom) {
	this->zoom = zoom;
}

void Camera::Translate(const glm::vec3& translation) {
	position += translation;
}

void Camera::Rotate(float rotation) {
	this->rotation += rotation;
}

void Camera::Zoom(float zoom) {
	this->zoom += zoom;
}

void Camera::SetShader(Shader* shader) {
	this->shader = shader;
}

void Camera::Use() {
	shader->use();
	shader->set("u_viewProjection", GetViewProjection());
}
