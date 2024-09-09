#include "gameobject.h"

using namespace Jenjin;

// Setters
void GameObject::set_transform(Transform transform) { this->transform = transform; }
void GameObject::set_position(glm::vec2 position) { transform.position = position; }
void GameObject::set_rotation(float rotation) { transform.rotation = rotation; }
void GameObject::set_scale(glm::vec2 scale) { transform.scale = scale; }
void GameObject::set_color(glm::vec3 color) { this->color = color; }
void GameObject::set_mesh_id(int mesh_id) { this->mesh_id = mesh_id; }
void GameObject::set_id(int id) { this->id = id; }
void GameObject::set_name(std::string name) { this->name = name; }
void GameObject::set_name_lua(const std::string& name) { this->name = name; }
void GameObject::set_meshdata(MeshData meshdata) { this->meshdata = meshdata; }

// Modifiers
void GameObject::translate(glm::vec2 translation) { transform.position += translation; }
void GameObject::rotate(float rotation) { transform.rotation += rotation; }
void GameObject::scale(glm::vec2 scale) { transform.scale *= scale; }

// Utility
void GameObject::rotate_towards(const glm::vec2& target) {
	transform.rotation = rotation_towards(target);
}

float GameObject::rotation_towards(const glm::vec2& target) {
	glm::vec2 diff = target - transform.position;
	float angle = glm::atan(diff.y, diff.x);
	float degrees = glm::mod(glm::degrees(-angle), 360.0f);
	return degrees;
}

float GameObject::distance_to(const glm::vec2& target) {
	return glm::distance(transform.position, target);
}
