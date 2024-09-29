#pragma once

#include "jenjin/mesh.h"

#include <glm/glm.hpp>

namespace Jenjin {
class GameObject {
public:
	GameObject(Mesh mesh) : mesh(mesh) {}
	~GameObject() = default;

	// Transform
	struct Transform {
		glm::vec2 position;
		glm::vec2 scale;
		float rotation;
	} transform = { glm::vec2(0.0f), glm::vec2(1.0f), 0.0f };

	// Appearance
	glm::vec3 color = glm::vec3(1.0f);

	// Mesh used to render in `Scene::render()`
	Mesh mesh;
	MeshReference* meshReference = nullptr;

	// TODO: hierarchy
	#ifdef false
	GameObject* parent = nullptr;
	std::vector<GameObject*> children = {};
	#endif
};
}
