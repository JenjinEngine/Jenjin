#include "luamanager.h"
#include "engine.h"
#include "sol/forward.hpp"
#include "sol/types.hpp"

#include <sol/protected_function_result.hpp>
#include <sol/raii.hpp>
#include <sol/resolve.hpp>
#include <spdlog/spdlog.h>
#include <sol/sol.hpp>

using namespace Jenjin;

LuaManager::LuaManager() {
	spdlog::trace("LuaManager::LuaManager()");

	state.open_libraries(
		sol::lib::base,
		sol::lib::math,
		sol::lib::debug,
		sol::lib::table,
		sol::lib::string,
		sol::lib::coroutine
	);

	state.set_function("error", [](const std::string& arg) {
		spdlog::error("[LUA] {}", arg);
	});

	state.set_function("warn", [](const std::string& arg) {
		spdlog::warn("[LUA] {}", arg);
	});

	state.set_function("info", [](const std::string& arg) {
		spdlog::info("[LUA] {}", arg);
	});

	state.set_function("print", [](const std::string& arg) {
		spdlog::info("[LUA] {}", arg);
	});

	state.set_function("debug", [](const std::string& arg) {
		spdlog::debug("[LUA] {}", arg);
	});

	this->bindings();
}

void LuaManager::bindings() {
	this->state.new_usertype<Engine_t>("Engine",
																		"add_scene", &Engine_t::add_scene,
																		"activate_scene", &Engine_t::activate_scene,
																		"launch", &Engine_t::launch,

																		"active_scene", &Engine_t::active_scene,
																		"running", &Engine_t::running,
																		"set_window", &Engine_t::set_window
																		);

	this->state.new_usertype<Scene>("Scene",
																 "add_gameobject", &Scene::add_gameobject,
																 "get_gameobject", &Scene::get_gameobject_ptr,
																 "build", &Scene::build,
																 "load", &Scene::load,
																 "update", &Scene::update,
																 "save", &Scene::save
																 );

	this->state.new_usertype<GameObject>("GameObject",
																			"set_transform", &GameObject::set_transform,
																			"set_position", &GameObject::set_position,
																			"set_rotation", &GameObject::set_rotation,
																			"set_scale", &GameObject::set_scale,
																			"set_color", &GameObject::set_color,
																			"set_mesh_id", &GameObject::set_mesh_id,
																			"set_name", &GameObject::set_name,
																			"set_texture", &GameObject::set_texture,
																			"get_transform", &GameObject::get_transform,
																			"get_position", &GameObject::get_position,
																			"get_rotation", &GameObject::get_rotation,
																			"get_scale", &GameObject::get_scale,
																			"get_color", &GameObject::get_color,
																			"get_mesh_id", &GameObject::get_mesh_id,
																			"get_name", &GameObject::get_name,
																			"remove_texture", &GameObject::remove_texture,
																			"translate", &GameObject::translate,
																			"rotate", &GameObject::rotate,
																			"scale", &GameObject::scale,
																			"rotate_towards", &GameObject::rotate_towards,
																			"rotation_towards", &GameObject::rotation_towards,
																			"distance_to", &GameObject::distance_to
																			);

	this->state.new_usertype<Transform>("Transform",
																		 "position", &Transform::position,
																		 "rotation", &Transform::rotation,
																		 "scale", &Transform::scale,

																		 "position", &Transform::position,
																		 "rotation", &Transform::rotation,
																		 "scale", &Transform::scale
																		 );

	this->state.new_usertype<Mesh>("Mesh",
																"vertices", &Mesh::vertices,
																"indices", &Mesh::indices
																);

	this->state.new_usertype<MeshReference>("MeshReference",
																				 "base_vertex", &MeshReference::base_vertex,
																				 "base_index", &MeshReference::base_index,
																				 "vertex_count", &MeshReference::vertex_count,
																				 "index_count", &MeshReference::index_count
																				 );

	this->state.new_usertype<Vertex>("Vertex",
																	"position", &Vertex::position,
																	"tex_coords", &Vertex::tex_coords
																	);

	this->state.new_usertype<glm::vec2>("vec2", sol::constructors<glm::vec2(float, float)>(),
																		 "x", &glm::vec2::x,
																		 "y", &glm::vec2::y
																		 );

	this->state.new_usertype<glm::vec3>("vec3",
																		 "x", &glm::vec3::x,
																		 "y", &glm::vec3::y,
																		 "z", &glm::vec3::z
																		 );

	this->state.new_usertype<glm::vec4>("vec4",
																		 "x", &glm::vec4::x,
																		 "y", &glm::vec4::y,
																		 "z", &glm::vec4::z,
																		 "w", &glm::vec4::w
																		 );

	this->state["print_name"] = [](GameObject* go) {
		spdlog::info("GameObject name: {}", go->name);
	};
}

void LuaManager::script(std::string code) {
	sol::protected_function_result result = state.safe_script(code);

	if (!result.valid()) {
		sol::error error = result;
		spdlog::error("[LUA] {}", error.what());
	}
}

void LuaManager::script_file(std::string file) {
	sol::protected_function_result result = state.safe_script_file(file);

	if (!result.valid()) {
		sol::error error = result;
		spdlog::error("[LUA] {}", error.what());
	}

	// Get the `ready` and `update` functions
	sol::protected_function ready = state["ready"];
	sol::protected_function update = state["update"];

	this->m_functions[file] = std::make_pair(ready, update);
}

void LuaManager::reload_files() {
	spdlog::trace("LuaManager::reload_files()");

	std::vector<std::string> paths;
	for (auto &pair : this->m_functions)
	paths.push_back(pair.first);

	this->m_functions.clear();

	this->state.stack_clear();

	for (auto &path : paths) {
		spdlog::debug("[LUA] Reloading file: {}", path);
		this->script_file(path);
	}
}

void LuaManager::ready() {
	spdlog::trace("LuaManager::ready()");
	for (auto &pair : this->m_functions) {
		auto result = pair.second.first();
		if (!result.valid()) {
			sol::error error = result;
			spdlog::error("[LUA] {}", error.what());
		}
	}
}

void LuaManager::update() {
	for (auto &pair : this->m_functions) {
		auto result = pair.second.second();
		if (!result.valid()) {
			sol::error error = result;
			spdlog::error("[LUA] {}", error.what());
		}
	}
}

void LuaManager::update_ptrs() {
	spdlog::trace("LuaManager::update_ptrs()");
	this->state["scene"] = Engine->active_scene;
}
