#ifndef JENJIN_GAMEOBJECT_H
#define JENJIN_GAMEOBJECT_H

#include "components/transform.h"
#include "components/mesh.h"

#include <string>
#include <spdlog/spdlog.h>

namespace Jenjin {
struct GameObject {
	GameObject(MeshData meshdata, std::string name) : meshdata(meshdata), name(name) {}

	void set_transform(Transform transform) { this->transform = transform; }
	void set_position(glm::vec2 position) { transform.position = position; }
	void set_rotation(float rotation) { transform.rotation = rotation; }

	void translate(glm::vec2 translation) { transform.position += translation; }
	void rotate(float rotation) { transform.rotation += rotation; }

	void fill_in_id(int id) { this->id = id; }

	Transform transform = { glm::vec2(0.0f, 0.0f) };
	MeshData meshdata;

	std::string name;

	glm::vec3 color = glm::vec3(0.8f, 0.8f, 1.0f);

	int mesh_id = -1;
	int id = -1;
};
}

#endif // JENJIN_GAMEOBJECT_H
