#ifndef JENJIN_SCENE_H
#define JENJIN_SCENE_H

#include <glm/glm.hpp>
#include <vector>

#include "camera.h"
#include "mesh.h"

namespace Jenjin {
class Scene {
public:
	Scene() = default;
	~Scene();

	void render();
	void add_mesh(MeshData mesh);
	void build();

private:
	unsigned int m_vao, m_vbo, m_ebo = 0;

	std::vector<Mesh> m_meshes;
	std::vector<Vertex> m_vertices;
	std::vector<unsigned int> m_indices;

	// Shader needs to be loaded in build, after the context is created
	Shader* m_shader = nullptr;

	Camera m_camera;
};
}

#endif // JENJIN_SCENE_H
