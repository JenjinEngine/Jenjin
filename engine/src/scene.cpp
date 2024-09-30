#include "jenjin/scene.h"
#include "jenjin/mesh.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>

#include <spdlog/spdlog.h>

#include <algorithm>

using namespace Jenjin;

Scene::Scene() {
	spdlog::trace("Scene::Scene()");
}

void Scene::SetTarget(Target* target) {
	this->target = target;
	this->camera.Resize(target->GetSize());
}

void Scene::AddGameObject(std::shared_ptr<GameObject> gameObject) {
	gameObjects.emplace_back(gameObject);
}

void Scene::RemoveGameObject(std::shared_ptr<GameObject> gameObject) {
	gameObjects.erase(std::remove(gameObjects.begin(), gameObjects.end(), gameObject), gameObjects.end());
}

void Scene::Build() {
	spdlog::trace("Scene::Build()");

	// Clear out any old data
	this->meshReferences.clear();
	this->meshReferences.reserve(gameObjects.size());

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	for (auto& gameObject : gameObjects) {
		int baseVertex = vertices.size();
		int baseIndex = indices.size();

		std::vector<Vertex>& meshVertices = gameObject->mesh.vertices;
		std::vector<GLuint>& meshIndices = gameObject->mesh.indices;

		MeshReference meshReference = { baseVertex, baseIndex, (int)meshIndices.size(), (int)meshVertices.size() };
		this->meshReferences.emplace_back(meshReference);

		vertices.insert(vertices.end(), meshVertices.begin(), meshVertices.end());
		indices.insert(indices.end(), meshIndices.begin(), meshIndices.end());

		gameObject->meshReference = &this->meshReferences.back();
	}

	spdlog::debug("Built {} mesh references... now generating buffers", this->meshReferences.size());

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	spdlog::debug("Generated buffers (vao: {}, vbo: {}, ebo: {})", vao, vbo, ebo);
}

void Scene::Update() {
	spdlog::trace("Scene::Update()");
}

void Scene::Render() {
	this->shader.use();
	this->camera.Use();
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, ebo);

	camera.Update();

	for (auto& gameObject : gameObjects) {
		MeshReference* meshReference = gameObject->meshReference;
		if (!meshReference) {
			spdlog::error("GameObject has no mesh reference");
			continue;
		}

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-gameObject->transform.position, 0.0f));
		model = glm::rotate(model, glm::radians(-gameObject->transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(gameObject->transform.scale, 1.0f));

		shader.set("u_model", model);

		glDrawElementsBaseVertex(GL_TRIANGLES, meshReference->indexCount, GL_UNSIGNED_INT, 0, meshReference->baseVertex);
	}
}
