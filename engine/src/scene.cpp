#include <glad/glad.h>
#include <GL/gl.h>
#include <spdlog/spdlog.h>

#include "scene.h"
#include "mesh.h"
#include "shader.h"

using namespace Jenjin;

Scene::~Scene() {
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);

	delete m_shader;
}

void Scene::build() {
	/* Shader shader = Shader("engine/shaders/vertex.glsl", "engine/shaders/fragment.glsl"); */
	/* this->m_shader = &shader; */
	this->m_shader = new Shader("engine/shaders/vertex.glsl", "engine/shaders/fragment.glsl");

	spdlog::debug("Creating camera");
	m_shader->use(); m_camera.setup_proj(*m_shader);

	spdlog::debug("Building scene");

	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(Vertex), m_vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), m_indices.data(), GL_STATIC_DRAW);

	// Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glEnableVertexAttribArray(0);
	// Texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Scene::add_mesh(MeshData mesh) {
	int base_vertex = (int)m_vertices.size();
	int base_index = (int)m_indices.size();

	std::vector<Vertex>& vertices = mesh.vertices;
	std::vector<unsigned int>& indices = mesh.indices;

	this->m_meshes.emplace_back(base_vertex, base_index, (int)vertices.size(), (int)indices.size());

	m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
	m_indices.insert(m_indices.end(), indices.begin(), indices.end());
}

void Scene::render() {
	this->m_shader->use();

	#ifndef NDEBUG
	m_camera.update_deltas();
	m_camera.processInput(glfwGetCurrentContext());
	#endif

	if (m_camera.has_new_projection())
		m_camera.setup_proj(*m_shader);

	m_camera.bind_uniforms(*m_shader);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	for (Mesh& mesh : m_meshes) {
		// TODO: Have the model matrix come from GameObject
		glm::mat4 model = glm::mat4(1.0f);

		m_shader->set("u_model", model);

		glDrawElementsBaseVertex(GL_TRIANGLES, mesh.index_count, GL_UNSIGNED_INT, 0, mesh.base_vertex);
	}
}
