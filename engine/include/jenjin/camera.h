#pragma once

#include "jenjin/shader.h"

#include <glm/glm.hpp>
#include <glm/fwd.hpp>

namespace Jenjin {
// 2D Orthographic Camera
// NOTE: one day hybrid 2D/3D camera?
class Camera {
public:
	Camera(Shader* shader, glm::vec2 size);

	void SetShader(Shader* shader);

	void SetPosition(const glm::vec3& position);
	void SetRotation(float rotation);
	void SetZoom(float zoom);

	void Translate(const glm::vec3& translation);
	void Rotate(float rotation);
	void Zoom(float zoom);

	void Resize(glm::vec2 size);

	void Update();
	void Use();

	const glm::vec3& GetPosition() const { return position; }
	const float GetRotation() const { return rotation; }
	const glm::mat4 GetViewProjection() const { return projection * view; }

	glm::vec3* GetPositionPointer() { return &position; }
	float* GetZoomPointer() { return &zoom; }
	float* GetRotationPointer() { return &rotation; }

private:
	Shader* shader;

	glm::vec3 position;
	float rotation;
	float zoom;

	float aspectRatio;

	glm::mat4 projection;
	glm::mat4 view;

	int ow, oh;
};
}
