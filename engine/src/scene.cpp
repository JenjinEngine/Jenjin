#define GLFW_INCLUDE_NONE

#include "jenjin/scene.h"
#include "jenjin/gameobject.h"
#include "jenjin/helpers.h"
#include "jenjin/mesh.h"

#include <glm/ext/matrix_transform.hpp>
#include <glm/fwd.hpp>

#include <spdlog/spdlog.h>

#include <algorithm>
#include <fstream>
#include <iostream>

using namespace Jenjin;

Scene::Scene() { spdlog::trace("Scene::Scene()"); }

void Scene::SetTarget(Target *target) {
  spdlog::trace("Scene::SetTarget({})", (void *)target);

  this->target = target;
  this->camera.Resize(target->GetSize());
}

void Scene::AddGameObject(std::shared_ptr<GameObject> gameObject) {
  gameObjects.emplace_back(gameObject);
}

void Scene::RemoveGameObject(std::shared_ptr<GameObject> gameObject) {
  this->RemoveGameObject(gameObject.get());
}
void Scene::RemoveGameObject(GameObject *gameObject) {
  gameObjects.erase(
      std::remove_if(gameObjects.begin(), gameObjects.end(),
                     [gameObject](std::shared_ptr<GameObject> go) {
                       return go.get() == gameObject;
                     }),
      gameObjects.end());
}

void Scene::SetGameObjectTexture(GameObject *gameObject,
                                 const std::string &texturePath) {

  if (texturePath.empty()) {
    gameObject->texturePath = "";
    return;
  }

  if (this->textures.find(texturePath) == this->textures.end()) {
    auto alpha = texturePath.find(".png") != std::string::npos;
    this->textures[texturePath] =
        std::make_shared<Texture>(texturePath.c_str(), alpha);
  }

  gameObject->texturePath = texturePath;
}

void Scene::SetGameObjectTexture(std::shared_ptr<GameObject> gameObject,
                                 const std::string &texturePath) {
  SetGameObjectTexture(gameObject.get(), texturePath);
}

void Scene::Build() {
  spdlog::trace("Scene::Build()");

  // Clear out any old data
  this->meshReferences.clear();
  this->meshReferences.reserve(gameObjects.size());

  // Clear out any old data
  this->meshReferences.clear();
  this->meshReferences.reserve(gameObjects.size());

  // Preallocate vectors with appropriate sizes
  std::vector<Vertex> vertices;
  std::vector<GLuint> indices;

  // Reserve enough space in vertices/indices to avoid resizing during
  // emplace_back
  size_t totalVertices = 0;
  size_t totalIndices = 0;
  for (const auto &gameObject : gameObjects) {
    totalVertices += gameObject->mesh.vertices.size();
    totalIndices += gameObject->mesh.indices.size();
  }
  vertices.reserve(totalVertices);
  indices.reserve(totalIndices);

  for (auto &gameObject : gameObjects) {
    int baseVertex = vertices.size();
    int baseIndex = indices.size();

    std::vector<Vertex> &meshVertices = gameObject->mesh.vertices;
    std::vector<GLuint> &meshIndices = gameObject->mesh.indices;

    MeshReference meshReference = {baseVertex, baseIndex,
                                   static_cast<int>(meshIndices.size()),
                                   static_cast<int>(meshVertices.size())};
    this->meshReferences.emplace_back(meshReference);

    // Insert vertices and indices directly into preallocated vectors
    vertices.insert(vertices.end(),
                    std::make_move_iterator(meshVertices.begin()),
                    std::make_move_iterator(meshVertices.end()));
    indices.insert(indices.end(), std::make_move_iterator(meshIndices.begin()),
                   std::make_move_iterator(meshIndices.end()));

    gameObject->meshReferenceID = this->meshReferences.size() - 1;
  }

  spdlog::debug("Built {} mesh references... now updating buffers",
                this->meshReferences.size());

  if (vao == 0 || vbo == 0 || ebo == 0) {
    spdlog::debug("Generating new buffers");
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);
  }

  glBindVertexArray(vao);

  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex),
               vertices.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
               indices.data(), GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, position));
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                        (void *)offsetof(Vertex, texCoord));

  glEnableVertexAttribArray(0);
  glEnableVertexAttribArray(1);

  spdlog::debug("Generated buffers (vao: {}, vbo: {}, ebo: {})", vao, vbo, ebo);
}

void Scene::Update() { this->luaManager.Update(); }

void Scene::Render() {
  this->shader.use();
  this->camera.Use();
  this->camera.Update();
  glBindVertexArray(vao);
  glBindBuffer(GL_ARRAY_BUFFER, ebo);

  for (auto &gameObject : gameObjects) {
    if (gameObject->meshReferenceID == -1) {
      spdlog::error("GameObject has no mesh reference");
      continue;
    }

    MeshReference *meshReference =
        &this->meshReferences[gameObject->meshReferenceID];

    glm::mat4 model = glm::mat4(1.0f);
    model =
        glm::translate(model, glm::vec3(gameObject->transform.position, 0.0f));
    model = glm::rotate(model, glm::radians(-gameObject->transform.rotation),
                        glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::scale(model, glm::vec3(gameObject->transform.scale, 1.0f));

    shader.set("u_model", model);
    shader.set("u_color", gameObject->color);

    if (!gameObject->texturePath.empty()) {
      this->textures[gameObject->texturePath]->bind(0);
      shader.set("u_texture", 0);
    }

    bool hasTexture = !gameObject->texturePath.empty();
    shader.set("u_hasTexture", hasTexture);
    shader.set("u_mixColor", gameObject->mixColor && hasTexture);

    glDrawElementsBaseVertex(GL_TRIANGLES, meshReference->indexCount,
                             GL_UNSIGNED_INT, 0, meshReference->baseVertex);
  }
}

std::shared_ptr<GameObject> Scene::GetGameObject(const std::string &name) {
  for (auto &go : this->gameObjects) {
    if (go->name == name) {
      return go;
    }
  }

  return nullptr;
}

struct GOBJSAVABLE {
  Jenjin::GameObject::Transform transform;
  glm::vec3 color;

  char texturePath[128];
  char name[128];
};

void Scene::Save(const std::string &path) {
  spdlog::trace("Scene::Save({})", path);

  std::ofstream file(path, std::ios::binary);
  Save(file);
}

void Scene::Save(std::ofstream &file) {
  spdlog::trace("Scene::Save({})", (void *)&file);

  if (this->gameObjects.empty()) {
    spdlog::debug("No game objects to save");
    return;
  }

  for (auto &go : this->gameObjects) {
    GOBJSAVABLE gobj = {.transform = go->transform,
                        .color = go->color,

                        .texturePath = {0},
                        .name = {0}};

    strncpy(gobj.name, go->name.c_str(), sizeof(gobj.name));
    auto path = go->texturePath;
    auto pos = path.find("textures/");
    if (pos != std::string::npos) {
      const char *text = path.c_str() + pos;
      spdlog::debug("Texture path is in the textures directory: {}", text);
      strncpy(gobj.texturePath, text, sizeof(gobj.texturePath));
    } else {
      spdlog::warn("Texture path is not in the textures directory: {}", path);
      strncpy(gobj.texturePath, path.c_str(), sizeof(gobj.texturePath));
    }

    gobj.name[sizeof(gobj.name) - 1] = 0;
    gobj.texturePath[sizeof(gobj.texturePath) - 1] = 0;

    file.write(reinterpret_cast<char *>(&gobj), sizeof(GOBJSAVABLE));
  }
}

void Scene::Load(const std::string &path) {
  spdlog::trace("Scene::Load({})", path);

  std::ifstream file(path, std::ios::binary);
  this->Load(file);
}

void Scene::Load(std::ifstream &file) {
  spdlog::trace("Scene::Load({})", (void *)&file);

  file.seekg(0, std::ios::end);
  int size = file.tellg();
  file.seekg(0, std::ios::beg);

  spdlog::debug("File size: {}", size);

  this->gameObjects.clear();

  for (GOBJSAVABLE gobj;
       file.read(reinterpret_cast<char *>(&gobj), sizeof(GOBJSAVABLE));) {
    spdlog::debug("Reading GOBJSAVABLE from file");

    auto go = std::make_shared<GameObject>(
        gobj.name, Jenjin::Helpers::CreateQuad(2.0f, 2.0f));
    go->transform = gobj.transform;
    go->color = gobj.color;
    go->texturePath = gobj.texturePath;

    // Load texture if it exists
    if (std::strlen(go->texturePath.data()) > 0) {
      this->SetGameObjectTexture(go, go->texturePath.data());
    }

    this->AddGameObject(go);
  }

  this->Build();
}
