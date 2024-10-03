#include "jenjin/camera.h"
#include "glm/trigonometric.hpp"

#include <spdlog/spdlog.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace Jenjin;

Camera::Camera(Shader* shader, glm::vec2 size) : shader(shader) {
	float& width = size.x; float& height = size.y;

	rotation = 0;
	zoom = 1;

	position = glm::vec3(0, 0, 0);

	// Set the camera's projection matrix
	Resize(size);
}

void Camera::Resize(glm::vec2 size) {
    float aspectRatio = size.x / size.y;
    float inverseZoom = 1.0f / (zoom / 10); // Invert to make zoom zoom in and also scale everything down...
    this->projection = glm::ortho(-aspectRatio * inverseZoom, aspectRatio * inverseZoom, -inverseZoom, inverseZoom, -1.0f, 1.0f);
}

void Camera::Update() {
	// include rotation
	view = glm::translate(glm::mat4(1.0f), -position) * glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1));
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
