#ifndef JENJIN_SCENE_H
#define JENJIN_SCENE_H

#include <glm/glm.hpp>
#include <functional>
#include <vector>

#include "components/mesh.h"

#include "camera.h"
#include "gameobject.h"

namespace Jenjin {
class Scene {
public:
	Scene(std::vector<GameObject*> gameobjects) : m_gameobjects(gameobjects) {}
	~Scene();

	void update();
	void render();

	void add_game_objects(std::vector<GameObject*> game_objects);
	void add_game_object(GameObject* game_object);
	int add_mesh(MeshData mesh);

	GameObject* get_gameobject(int id);
	GameObject* get_gameobject(std::string name);

	void build();

	void set_update_callback(std::function<void(Scene*)> update_function) { m_update_function = update_function; }

	#ifndef NDEBUG
	std::vector<GameObject*> m_gameobjects;
	#endif

private:
	std::function<void(Scene*)> m_update_function = nullptr;

	unsigned int m_vao, m_vbo, m_ebo = 0;

	#ifdef NDEBUG
	std::vector<GameObject*> m_gameobjects;
	#endif

	std::vector<Mesh> m_meshes;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	// Shader needs to be loaded in build, after the context is created
	Shader* m_shader = nullptr;

	Camera m_camera;
};
}

#endif // JENJIN_SCENE_H
