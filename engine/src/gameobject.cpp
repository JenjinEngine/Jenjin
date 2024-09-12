#include "gameobject.h"
#include "engine.h"

using namespace Jenjin;

Mesh create_quad(float width, float height) {
	Mesh mesh = {};

	mesh.vertices = {
		// Position, Texture coordinates
		{ glm::vec2(-width / 2.0f, -height / 2.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec2(width / 2.0f, -height / 2.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec2(width / 2.0f, height / 2.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec2(-width / 2.0f, height / 2.0f), glm::vec2(0.0f, 1.0f) }
	};

	mesh.indices = {
		0, 1, 2,
		2, 3, 0
	};

	return mesh;
}

GameObject::GameObject(std::string name) {
	this->name = name;
	this->mesh = create_quad(1.0f, 1.0f);
}

// Setters
void GameObject::set_transform(Transform transform) { this->transform = transform; }
void GameObject::set_position(glm::vec2 position) { transform.position = position; }
void GameObject::set_rotation(float rotation) { transform.rotation = rotation; }
void GameObject::set_scale(glm::vec2 scale) { transform.scale = scale; }
void GameObject::set_color(glm::vec3 color) { this->color = color; }
void GameObject::set_mesh_id(int mesh_id) { this->mesh_id = mesh_id; }
void GameObject::set_name(std::string name) { this->name = name; }

void GameObject::set_texture(std::string path, bool alpha) {
	this->texture_path = path;
	this->alpha = alpha;

	if (Engine.running) {
		Engine.active_scene->load_gameobject_texture(this);
	}
}

// Modifiers
void GameObject::translate(glm::vec2 translation) { transform.position += translation; }
void GameObject::rotate(float rotation) { transform.rotation += rotation; transform.rotation = glm::mod(transform.rotation, 360.0f); }
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
