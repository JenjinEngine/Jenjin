#include "scene.h"
#include "mesh.h"

#include <spdlog/spdlog.h>

using namespace Jenjin;

Scene::Scene() {
	spdlog::debug("Scene created");
}

Scene::Scene(std::vector<std::shared_ptr<GameObject>> game_objects) {
	for (auto& go : game_objects) {
		this->add_gameobject(go);
	}
}

Scene::~Scene() {
	spdlog::debug("Destroying scene");

	glDeleteVertexArrays(1, &this->m_vao);
	glDeleteBuffers(1, &this->m_vbo);
	glDeleteBuffers(1, &this->m_ebo);

	spdlog::debug("Scene destroyed");
}

void Scene::build() {
	spdlog::debug("Building scene buffers");
	std::vector<unsigned int> scene_indices;
	std::vector<Vertex> scene_vertices;

	for (auto& go : this->m_game_objects) {
		int base_vertex = (int)scene_vertices.size();
		int base_index = (int)scene_indices.size();
		std::vector<Vertex>& vertices = go->mesh.vertices;
		std::vector<unsigned int>& indices = go->mesh.indices;

		MeshReference meshref = { base_vertex, base_index, (int)vertices.size(), (int)indices.size() };
		this->m_mesh_references.push_back(meshref);

		scene_vertices.insert(scene_vertices.end(), vertices.begin(), vertices.end());
		scene_indices.insert(scene_indices.end(), indices.begin(), indices.end());

		// Assign the mesh ID to the game object
		go->mesh_id = (int)this->m_mesh_references.size() - 1;
	}

	spdlog::debug("Scene triangles: {}", scene_indices.size() / 3);

	spdlog::debug("Generating scene VAO");
	glGenVertexArrays(1, &this->m_vao);
	glBindVertexArray(this->m_vao);

	spdlog::debug("Generating scene VBO");
	glGenBuffers(1, &this->m_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, this->m_vbo);
	glBufferData(GL_ARRAY_BUFFER, scene_vertices.size() * sizeof(Vertex), &scene_vertices[0], GL_STATIC_DRAW);

	spdlog::debug("Generating scene EBO");
	glGenBuffers(1, &this->m_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, scene_indices.size() * sizeof(unsigned int), &scene_indices[0], GL_STATIC_DRAW);

	// Setup vertex attributes
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, tex_coords));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	spdlog::debug("Scene buffers built");
}

void Scene::update() {
	if (this->m_update_callback) {
		this->m_update_callback(this);
	}
}

void Scene::render() {
	if (this->m_render_callback) {
		this->m_render_callback(this);
	}

	this->m_default_shader.use();

#ifndef NDEBUG
	m_default_camera.processInput(glfwGetCurrentContext());
#endif

	if (this->m_render_callback)
		this->m_render_callback(this);

	m_default_camera.setup_proj(m_default_shader);
	m_default_camera.bind_uniforms(m_default_shader);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	for (auto& gobj : m_game_objects) {
		auto& meshref = m_mesh_references[gobj->mesh_id];

		glm::mat4 model = glm::scale(glm::mat4(1.0f), glm::vec3(gobj->transform.scale.x, gobj->transform.scale.y, 1.0f));
		model = glm::translate(model, glm::vec3(gobj->transform.position.x, gobj->transform.position.y, 0.0f));
		model = glm::rotate(model, -glm::radians(gobj->transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f));

		m_default_shader.set("u_model", model);

		glDrawElementsBaseVertex(GL_TRIANGLES, meshref.index_count, GL_UNSIGNED_INT, 0, meshref.base_vertex);
	}
}

void Scene::add_gameobject(std::shared_ptr<GameObject> game_object) {
	this->m_game_objects.push_back(game_object);
}

void Scene::add_gameobjects(std::vector<std::shared_ptr<GameObject>> game_objects) {
	for (auto& go : game_objects) {
		this->add_gameobject(go);
	}
}
