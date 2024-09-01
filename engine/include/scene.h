#ifndef JENJIN_SCENE_H
#define JENJIN_SCENE_H

#include <glm/glm.hpp>
#include <vector>

#include "components/mesh.h"

#include "camera.h"
#include "gameobject.h"

namespace Jenjin {
class Scene {
public:
	Scene(std::vector<GameObject*> game_objects) : m_game_objects(game_objects) {}
	~Scene();

	void render();

	void add_game_objects(std::vector<GameObject*> game_objects);
	void add_game_object(GameObject* game_object);
	int add_mesh(MeshData mesh);

	void build();

private:
	unsigned int m_vao, m_vbo, m_ebo = 0;

	std::vector<GameObject*> m_game_objects;

	std::vector<Mesh> m_meshes;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	// Shader needs to be loaded in build, after the context is created
	Shader* m_shader = nullptr;

	Camera m_camera;
};
}

#endif // JENJIN_SCENE_H
