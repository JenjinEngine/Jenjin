#include <glad/glad.h>
#include <GL/gl.h>
#include <ios>
#include <spdlog/spdlog.h>
#include <sstream>
#include <thread>

#include "scene.h"
#include "components/mesh.h"
#include "gameobject.h"
#include "glm/ext/matrix_transform.hpp"
#include "lua.h"
#include "shader.h"
#include "state.h"

using namespace Jenjin;

Scene::~Scene() {
	for (GameObject* gobj : m_gameobjects) {
		spdlog::debug("Deleting gameobject: {}", gobj->name);
		delete gobj;
	}

	spdlog::debug("Deleting buffers");
	glDeleteVertexArrays(1, &m_vao);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);

	spdlog::debug("Deleting shader");
	delete m_shader;
}

void Scene::build() {
	this->m_meshes.clear();
  this->m_indices.clear();
	this->m_vertices.clear();

	static int count = 0;
	spdlog::debug("Building scene {}", count++);

	for (GameObject* gobj : m_gameobjects) {
		MeshData meshdata = gobj->meshdata;
		int mesh_id = add_mesh(meshdata);
		gobj->mesh_id = mesh_id;
	}

	if (this->m_shader == nullptr)
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

void Scene::add_gameobjects(std::vector<GameObject*> game_objects) {
	this->m_gameobjects.reserve(game_objects.size());

	for (GameObject* gobj : game_objects)
		add_gameobject(gobj);
}


void Scene::add_gameobject(GameObject* game_object) {
	game_object->id = (int)m_gameobjects.size() - 1;
	m_gameobjects.emplace_back(game_object);
}

int Scene::add_mesh(MeshData mesh) {
	int base_vertex = (int)m_vertices.size();
	int base_index = (int)m_indices.size();

	std::vector<Vertex>& vertices = mesh.vertices;
	std::vector<unsigned int>& indices = mesh.indices;

	Mesh l_mesh = { base_vertex, base_index, (int)vertices.size(), (int)indices.size() };
	this->m_meshes.push_back(l_mesh);

	m_vertices.insert(m_vertices.end(), vertices.begin(), vertices.end());
	m_indices.insert(m_indices.end(), indices.begin(), indices.end());

	return (int)m_meshes.size() - 1;
}

void Scene::update() {
	if (m_update_function != nullptr)
		m_update_function(this);
}

void Scene::render() {
	this->m_shader->use();

#ifndef NDEBUG
	m_camera.processInput(glfwGetCurrentContext());
#endif

	if (m_camera.m_changed_projection) {
		m_camera.setup_proj(*m_shader);
		m_camera.m_changed_projection = false;
	}

	m_camera.bind_uniforms(*m_shader);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	for (GameObject* gobj : m_gameobjects) {
		// PERF: Cache the model matrix
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(gobj->transform.position, gobj->transform.z_index));
		model = glm::rotate(model, glm::radians(-gobj->transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		m_shader->set("u_model", model);
		m_shader->set("go_color", gobj->color);

		Mesh* mesh = &m_meshes[gobj->mesh_id];

		/* spdlog::debug("Renderring GOBJ with name `{}` and colour ({}, {}, {})", gobj->name, gobj->color.x, gobj->color.y, gobj->color.z); */
		glDrawElementsBaseVertex(GL_TRIANGLES, mesh->index_count, GL_UNSIGNED_INT, 0, mesh->base_vertex);
	}
}

GameObject* Scene::get_gameobject(int id) {
	for (GameObject* gobj : m_gameobjects) {
		if (gobj->id == id)
			return gobj;
	}

	return nullptr;
}

GameObject* Scene::get_gameobject(std::string name) {
	for (GameObject* gobj : m_gameobjects) {
		if (gobj->name == name) {
			return gobj;
		}
	}

	spdlog::debug("Could not find gameobject with name {}", name);

	return nullptr;
}
