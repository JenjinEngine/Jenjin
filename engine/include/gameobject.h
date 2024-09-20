#ifndef JENJIN_GAMEOBJECT_H
#define JENJIN_GAMEOBJECT_H

#include "mesh.h"
#include "transform.h"

#include <string>

namespace Jenjin {
// A GameObject is a game entity that can be rendered
// and updated. It has a transform, color, and mesh.
// It can be manipulated by the user.
//
// It's important to note that the mesh reference is
// not stored in the GameObject. Instead, the mesh
// reference is stored in the scene. The `mesh_id`
// is an index into the mesh references vector in the
// scene.
class GameObject {
public:
	// Constructors
	GameObject(std::string name);

	// Setters
	void set_transform(Transform transform);
	void set_position(glm::vec2 position);
	void set_rotation(float rotation);
	void set_scale(glm::vec2 scale);
	void set_color(glm::vec3 color);
	void set_mesh_id(int mesh_id);
	void set_name(std::string name);

	void set_texture(std::string path, bool alpha = false, void* scene = nullptr);
	void remove_texture();

	// Getters
	Transform get_transform();
	glm::vec2 get_position();
	float get_rotation();
	glm::vec2 get_scale();
	glm::vec3 get_color();
	int get_mesh_id();
	std::string get_name();

	// Modifiers
	void translate(glm::vec2 translation);
	void rotate(float rotation);
	void scale(glm::vec2 scale);

	// Utility
	void rotate_towards(const glm::vec2& target);
	float rotation_towards(const glm::vec2& target);
	float distance_to(const glm::vec2& target);

	// Data
	glm::vec3 color = glm::vec3(0.8f, 0.8f, 1.0f);
	Transform transform;
	std::string name;

	Mesh mesh;
	std::string texture_path = "";
	bool alpha = false;

	// `mesh_id` becomes an index into the mesh references
	// vector stored in the scene
	int mesh_id = -1;

	// `texture_id` becomes an index into the texture vector
	// stored in the scene
	int texture_id = -1;
};
}

#endif // JENJIN_GAMEOBJECT_H
