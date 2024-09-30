#pragma once

#include "jenjin/gameobject.h"
#include "jenjin/camera.h"
#include "jenjin/shader.h"
#include "jenjin/target.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <memory>
#include <vector>

namespace Jenjin {
class Scene {
public:
	Scene();
	~Scene() = default;

	void SetTarget(Target* target);

	void AddGameObject(std::shared_ptr<GameObject> gameObject);
	void RemoveGameObject(std::shared_ptr<GameObject> gameObject);

	void Build();

	void Update();
	void Render();

	Camera* GetCamera() { return &camera; }
	std::vector<std::shared_ptr<GameObject>>* GetGameObjects() { return &gameObjects; }
	Target* GetTarget() { return target; }

private:
	GLuint vao, vbo, ebo = 0;

	std::vector<std::shared_ptr<GameObject>> gameObjects = {};
	std::vector<MeshReference> meshReferences = {};

	Shader shader = Shader("shaders/default_vert.glsl", "shaders/default_frag.glsl");
	Target* target = nullptr;

	Camera camera = Camera(&shader, glm::vec2(800, 600));
};
}
