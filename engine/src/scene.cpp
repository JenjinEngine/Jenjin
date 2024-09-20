#include "scene.h"
#include "engine.h"
#include "ext/matrix_transform.hpp"
#include "fwd.hpp"
#include "gameobject.h"
#include "gtc/type_ptr.hpp"
#include "mesh.h"

#include <fstream>

#include <spdlog/spdlog.h>
#include <imgui.h>

using namespace Jenjin;

Scene::Scene() {
	spdlog::trace("Scene::Scene()");
}

Scene::Scene(std::string path) {
	spdlog::trace("Scene::Scene(\"{}\")", path);
	std::ifstream is(path, std::ios::binary);
	this->load(is);
}

Scene::Scene(std::vector<std::shared_ptr<GameObject>> game_objects) {
	spdlog::trace("Scene::Scene(vec<{}>)", game_objects.size());

	for (auto& go : game_objects) {
		this->add_gameobject(go);
	}
}

Scene::~Scene() {
	spdlog::trace("Scene::~Scene()");

	glDeleteVertexArrays(1, &this->m_vao);
	glDeleteBuffers(1, &this->m_vbo);
	glDeleteBuffers(1, &this->m_ebo);
}

void Scene::resize(GLFWwindow* window, int width, int height) {
	spdlog::debug("Resizing scene to {}x{}", width, height);

	if (this->m_resize_callback)
		this->m_resize_callback(this, window, width, height);

	glViewport(0, 0, width, height);
	m_default_camera.set_aspect_ratio((float)width / (float)height);
	m_default_camera.setup_proj(m_default_shader);
}

void Scene::build() {
	// Load all textures for the game objects
	// (only if the engine isn't running)
	//
	// Loading textures mid-game is slow and
	// can cause stuttering.
	if (!Engine->running) {
		for (auto& go : this->m_game_objects) {
			if (!go->texture_path.empty()) {
				auto texture = std::make_shared<Texture>(go->texture_path.c_str(), go->alpha);
				this->m_textures[go->texture_path] = texture;
				go->texture_id = (int)this->m_textures.size() - 1;
				spdlog::debug("Loaded texture for GameObject: {}", go->name);
			}
		}
	}

	// Clear the mesh references
	this->m_mesh_references.clear();

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
		spdlog::debug("Stored mesh reference");

		scene_vertices.insert(scene_vertices.end(), vertices.begin(), vertices.end());
		scene_indices.insert(scene_indices.end(), indices.begin(), indices.end());
		spdlog::debug("Inserted vertices and indices into scene buffers ready for building");

		// Assign the mesh ID to the game object
		go->mesh_id = (int)this->m_mesh_references.size() - 1;
		spdlog::debug("Assigned mesh ID: {}", go->mesh_id);
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

	if (!Engine->running) {
		this->m_lua_manager.script_file("test.lua");
	}

	/* spdlog::debug("Readying lua manager"); */
	/* this->m_lua_manager.ready(); */
}

void Scene::update() {
	if (this->m_update_callback) {
		this->m_update_callback(this);
	}

	static bool ready = false;
	if (!ready) {
		this->m_lua_manager.ready();
		ready = true;
	}

	if (Engine->active_scene != nullptr)
		this->m_lua_manager.update();
}

void Scene::render() {
	if (this->m_render_callback) {
		this->m_render_callback(this);
	}

	this->m_default_shader.use();

/* #ifndef NDEBUG */
	m_default_camera.processInput(glfwGetCurrentContext());
/* #endif */

	m_default_camera.setup_proj(m_default_shader);
	m_default_camera.bind_uniforms(m_default_shader);

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);

	for (auto& gobj : m_game_objects) {
		if (gobj->mesh_id == -1) continue;

		auto& meshref = m_mesh_references[gobj->mesh_id];

		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, glm::vec3(gobj->transform.position.x * 0.1f, gobj->transform.position.y * 0.1f, 0.0f));
		model = glm::rotate(model, -glm::radians(gobj->transform.rotation), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(gobj->transform.scale.x, gobj->transform.scale.y, 1.0f));

		m_default_shader.set("u_model", model);

		glm::vec3 color = gobj->color;
		m_default_shader.set("u_color", color);

		if (gobj->texture_id == -1) {
			m_default_shader.set("u_texture", -1);
			m_default_shader.set("u_use_texture", false);
		} else {
			m_default_shader.set("u_use_texture", true);

			auto texture = m_textures[gobj->texture_path];
			if (texture) {
				texture->bind(0);
				m_default_shader.set("u_texture", 0);
			}
		}

		glDrawElementsBaseVertex(GL_TRIANGLES, meshref.index_count, GL_UNSIGNED_INT, 0, meshref.base_vertex);
	}
}

void Scene::debug_menu(bool separate_window) {
	if (separate_window) {
		ImGui::Begin("Scene");
	} else {
		if (!ImGui::CollapsingHeader("Scene")) {
			return;
		}

		ImGui::Indent();
	}

	ImGui::Text("Game objects: %d", (int)this->m_game_objects.size());
	ImGui::Text("Mesh references: %d", (int)this->m_mesh_references.size());

	// Data uploaded to the GPU:
	// - Vertices (position, tex_coords : 4, 4 bytes)
	// - Indices (unsigned int : 4 bytes)
	// - Total size = (vertices + indices) * sizeof(float)
	size_t vram_bytes = (this->m_mesh_references.size() * sizeof(Vertex) + this->m_mesh_references.size() * sizeof(unsigned int));

	if (vram_bytes > 1024 * 1024) {
		ImGui::Text("VRAM: %.2f MB", vram_bytes / (1024.0f * 1024.0f));
	} else if (vram_bytes > 1024) {
		ImGui::Text("VRAM: %.2f KB", vram_bytes / 1024.0f);
	} else {
		ImGui::Text("VRAM: %d bytes", (int)vram_bytes);
	}
	ImGui::SameLine();
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered()) {
		ImGui::BeginTooltip();
		ImGui::Text("Total VRAM used by scene buffers, does not include textures");
		ImGui::EndTooltip();
	}

	if (ImGui::TreeNode("Utilities")) {
		if (ImGui::TreeNode("Create")) {
			static char game_object_name[128] = { 0 };
			ImGui::InputText("GameObject name", game_object_name, sizeof(game_object_name));

			if (ImGui::Button("Create GameObject")) {
				auto go = std::make_shared<GameObject>(game_object_name);
				this->add_gameobject(go);
			}

			static int many_count = 100;
      ImGui::InputInt("Count", &many_count);
			ImGui::SameLine();
			if (ImGui::SmallButton("Create")) {
				for (int i = 0; i < many_count; i++) {
					auto go = std::make_shared<GameObject>(game_object_name);
					int x = rand() % 800 - 400;
					int y = rand() % 800 - 400;

					go->set_position(glm::vec2(x, y));

					this->add_gameobject(go);
				}
			}

			ImGui::TreePop();
		}

		if (ImGui::Button("Build scene")) {
			this->build();
		}

		if (ImGui::Button("All GameObjects to origin")) {
			for (auto& go : this->m_game_objects) {
				go->transform.position = glm::vec2(0.0f);
			}
		}

		ImGui::TreePop();
	}

	if (ImGui::TreeNode("Game objects")) {
		int it = 0;
		for (auto& go : this->m_game_objects) {
			ImGui::PushID(it++);

			if (ImGui::CollapsingHeader(go->name.c_str())) {
				ImGui::Indent();
				if (ImGui::TreeNode("Transform")) {
					ImGui::DragFloat2("Position", glm::value_ptr(go->transform.position));
					ImGui::DragFloat("Rotation", &go->transform.rotation);
					ImGui::DragFloat2("Scale", glm::value_ptr(go->transform.scale));
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Color")) {
					ImGui::ColorEdit3("Color", glm::value_ptr(go->color));
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Mesh")) {
					ImGui::Text("Vertices: %d", (int)go->mesh.vertices.size());
					ImGui::Text("Indices: %d", (int)go->mesh.indices.size());
					ImGui::Text("Mesh ID: %d", go->mesh_id);
					ImGui::TreePop();
				}

				if (ImGui::TreeNode("Texture")) {
					static char texture_path[128] = { 0 };
					static bool alpha_channel_enabled = false;

					ImGui::Checkbox("Alpha", &alpha_channel_enabled);
					ImGui::SameLine();
					ImGui::InputText("Texture path", texture_path, sizeof(texture_path));

					if (ImGui::Button("Set texture")) {
						go->set_texture(texture_path, alpha_channel_enabled);
					}

					ImGui::SameLine();

					if (ImGui::Button("Remove texture")) {
						go->remove_texture();
					}

					ImGui::TreePop();
				}

				ImGui::Unindent();
			}
			ImGui::PopID();
		}

		ImGui::TreePop();
	}

	if (separate_window) {
		ImGui::End();
	} else {
		ImGui::Unindent();
	}
}

void Scene::add_gameobject(std::shared_ptr<GameObject> game_object) {
	this->m_game_objects.push_back(game_object);
	spdlog::debug("Added GameObject: {} ({})", game_object->name, (int)this->m_game_objects.size());
}

void Scene::add_gameobjects(std::vector<std::shared_ptr<GameObject>> game_objects) {
	for (auto& go : game_objects) {
		this->add_gameobject(go);
	}
}

GameObject* Scene::get_gameobject_ptr(const std::string& name) {
	for (auto& go : this->m_game_objects) {
		if (go->name == name) {
			return go.get();
		}
	}

	return nullptr;
}

std::shared_ptr<GameObject> Scene::get_gameobject(std::string name) {
	for (auto& go : this->m_game_objects) {
		if (go->name == name) {
			return go;
		}
	}

	return nullptr;
}

// This function is used to dynamically load textures for game objects
// that have details set after the scene has been built.
void Scene::load_gameobject_texture(GameObject* game_object) {
	if (this->m_textures.find(game_object->texture_path) != this->m_textures.end()) {
		spdlog::debug("Texture already loaded for GameObject: {}", game_object->name);

		// Set a preloaded texture
		int index = 0;
		for (auto& [key, value] : this->m_textures) {
			if (key == game_object->texture_path) {
				game_object->texture_id = index;
				break;
			}
			index++;
		}

		return;
	}

	auto texture = std::make_shared<Texture>(game_object->texture_path.c_str(), game_object->alpha);
	this->m_textures[game_object->texture_path] = texture;
	game_object->texture_id = (int)this->m_textures.size() - 1;
	spdlog::debug("Loaded texture for GameObject: {}", game_object->name);
}

void Scene::set_update_callback(std::function<void(Scene*)> callback) {
	this->m_update_callback = callback;
}

void Scene::set_render_callback(std::function<void(Scene*)> callback) {
	this->m_render_callback = callback;
}

void Scene::set_resize_callback(std::function<void(Scene*, GLFWwindow*, int, int)> callback) {
	this->m_resize_callback = callback;
}

// Save in binary format the game objects
// 128 + 2*4 + 4 + 2*4 + 3*4 + 128 + 1 = 289
void Scene::save(std::ostream& os) {
	if (this->m_game_objects.size() == 0) {
		spdlog::debug("No game objects to save");
		return;
	}

	for (auto& go : this->m_game_objects) {
		os.write(go->name.c_str(), 128);
		os.write((char*)&go->transform.position, 2 * sizeof(float));
		os.write((char*)&go->transform.rotation, sizeof(float));
		os.write((char*)&go->transform.scale, 2 * sizeof(float));
		os.write((char*)&go->color, 3 * sizeof(float));
		os.write(go->texture_path.c_str(), 128);
		os.write((char*)&go->alpha, 1);
	}
}

// Load in binary format the game objects
void Scene::load(std::istream& is) {
	spdlog::debug("Loading scene from file");

	// get the size of the file
	int begin = is.tellg();
	is.seekg(0, std::ios::end);
	int end = is.tellg();

	if (end - begin <= 1) {
		spdlog::debug("File is empty");
		return;
	}

	int gobj_count = (end - begin) / 289;
	int gobjs = 0;

	is.seekg(0, std::ios::beg);

	for (int i = 0; i < gobj_count; i++) {
		char name[128];
		glm::vec2 position;
		float rotation;
		glm::vec2 scale;
		glm::vec3 color;
		char texture_path[128];
		bool alpha;
		is.read(name, 128);
		is.read((char*)&position, 2 * sizeof(float));
		is.read((char*)&rotation, sizeof(float));
		is.read((char*)&scale, 2 * sizeof(float));
		is.read((char*)&color, 3 * sizeof(float));
		is.read(texture_path, 128);
		is.read((char*)&alpha, 1);
		auto go = std::make_shared<GameObject>(name);
		go->transform.position = position;
		go->transform.rotation = rotation;
		go->transform.scale = scale;
		go->color = color;
		go->texture_path = texture_path;

		if (!go->texture_path.empty())
			go->set_texture(texture_path, alpha, this);

		this->add_gameobject(go);
	}

	this->build();
}

void Scene::update_lua_ptrs() {
	this->m_lua_manager.update_ptrs();
	this->m_lua_manager.ready();
}
