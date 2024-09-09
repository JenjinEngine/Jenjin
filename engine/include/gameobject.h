#ifndef JENJIN_GAMEOBJECT_H
#define JENJIN_GAMEOBJECT_H

#include "components/transform.h"
#include "components/mesh.h"

#include <glm/trigonometric.hpp>
#include <string>
#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

namespace Jenjin {
class GameObject {
public:
	GameObject(MeshData meshdata, std::string name) : meshdata(meshdata), name(name) {}

	void set_transform(Transform transform);
	void set_position(glm::vec2 position);
	void set_rotation(float rotation);
	void set_scale(glm::vec2 scale);
	void set_color(glm::vec3 color);
	void set_mesh_id(int mesh_id);
	void set_id(int id);
	void set_name(std::string name);
	void set_name_lua(const std::string& name);
	void set_meshdata(MeshData meshdata);

	void translate(glm::vec2 translation);
	void rotate(float rotation);
	void scale(glm::vec2 scale);

	void rotate_towards(const glm::vec2& target);
	float rotation_towards(const glm::vec2& target);
	float distance_to(const glm::vec2& target);

	Transform transform = { glm::vec2(0.0f, 0.0f) };
	MeshData meshdata;
	std::string name;
	glm::vec3 color = glm::vec3(0.8f, 0.8f, 1.0f);
	int mesh_id = -1;
	int id = -1;
};
}

#endif // JENJIN_GAMEOBJECT_H
