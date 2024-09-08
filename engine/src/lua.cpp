#include "lua.h"
#include "state.h"
#include "utils.h"
#include "shapes.h"

#include <cstring>

#include <GLFW/glfw3.h>

#include <fmt/color.h>

#include <spdlog/spdlog.h>
#include <glm/glm.hpp>

#include <sol/sol.hpp>
#include <sol/raii.hpp>

#define BIND_ENUM(key) m_lua[#key] = key

Lua::Lua() {
	// disable gc for now m_lua.is_gc_on()
	spdlog::debug("Initializing Lua with GC enabled: {}", m_lua.is_gc_on());
	m_lua.stop_gc();
	m_lua.change_gc_mode_generational(0, 0);
	spdlog::debug("Initialized Lua with GC enabled: {}", m_lua.is_gc_on());

	m_lua.open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::table, sol::lib::package, sol::lib::math, sol::lib::os, sol::lib::io, sol::lib::string);

	{ // Logging
		m_lua.set_function("print", [](const std::string& str) {
			spdlog::debug("[{}] {}", fmt::format(fmt::fg(fmt::color::light_blue), "lua"), str);
		});

		m_lua.set_function("error", [](const std::string& str) {
			spdlog::error("[{}] {}", fmt::format(fmt::fg(fmt::color::pale_violet_red), "lua"), str);
		});

		m_lua.set_function("warn", [](const std::string& str) {
			spdlog::warn("[{}] {}", fmt::format(fmt::fg(fmt::color::light_yellow), "lua"), str);
		});

		m_lua.set_function("info", [](const std::string& str) {
			spdlog::info("[{}] {}", fmt::format(fmt::fg(fmt::color::light_green), "lua"), str);
		});

		m_lua.set_function("debug", [](const std::string& str) {
			spdlog::debug("[{}] {}", fmt::format(fmt::fg(fmt::color::light_blue), "lua"), str);
		});

		m_lua.set_function("trace", [](const std::string& str) {
			spdlog::trace("[{}] {}", fmt::format(fmt::fg(fmt::color::light_blue), "lua"), str);
		});
	}

	{ // GLM data type
		// vec2
		m_lua.new_usertype<glm::vec2>("vec2", sol::constructors<glm::vec2(), glm::vec2(float), glm::vec2(float, float)>(),
																"x", &glm::vec2::x, "y", &glm::vec2::y);

		// vec3
		m_lua.new_usertype<glm::vec3>("vec3", sol::constructors<glm::vec3(), glm::vec3(float), glm::vec3(float, float, float)>(),
																"x", &glm::vec3::x, "y", &glm::vec3::y, "z", &glm::vec3::z);

		m_lua["normalize"] = [](const glm::vec2& vec) {
			return glm::normalize(vec);
		};

		m_lua["normalize_v2"] = [](const glm::vec2& vec) {
			return glm::normalize(vec);
		};

		m_lua["normalize_v3"] = [](const glm::vec3& vec) {
			return glm::normalize(vec);
		};
	}

	{ // Engine data types
		// GameObject
		// use the `new_lua` method on the GameObject class to create a new object (it needs to be heap allocated)
		m_lua.new_usertype<GameObject>("GameObject",
																 sol::factories([](const MeshData& meshData, const std::string& name) {
																 GameObject* obj = new GameObject(meshData, name);
																 return std::shared_ptr<GameObject>(obj); // return a shared_ptr to the object (which is on the heap)
																 }),

																 "id", &GameObject::id,
																 "name", &GameObject::name,
																 "transform", &GameObject::transform,
																 "mesh_id", &GameObject::mesh_id,
																 "color", &GameObject::color);

		/* m_lua["GameObject"]["new_lua"] = &GameObject::new_lua; */
		m_lua["GameObject"]["set_transform"] = &GameObject::set_transform;
		m_lua["GameObject"]["translate"] = &GameObject::translate;
		m_lua["GameObject"]["rotate_towards"] = &GameObject::rotate_towards;
		m_lua["GameObject"]["set_position"] = &GameObject::set_position;
		m_lua["GameObject"]["set_rotation"] = &GameObject::set_rotation;
		m_lua["GameObject"]["rotate"] = &GameObject::rotate;
		m_lua["GameObject"]["fill_in_id"] = &GameObject::fill_in_id;

		// Transform
		m_lua.new_usertype<Transform>("Transform",
																"position", &Transform::position,
																"rotation", &Transform::rotation,
																"scale", &Transform::scale,
																"z_index", &Transform::z_index);

		// Mesh
		m_lua.new_usertype<Mesh>("Mesh",
													 "index_count", &Mesh::index_count,
													 "base_vertex", &Mesh::base_vertex);

		// MeshData
		m_lua.new_usertype<MeshData>("MeshData",
															 "vertices", &MeshData::vertices,
															 "indices", &MeshData::indices);

		// Scene
		m_lua.new_usertype<Scene>("Scene",
														"m_gameobjects", &Scene::m_gameobjects);

		// Functions for Scene (called through state.scene:func())
		m_lua["Scene"]["get_gameobject"] = &Scene::get_gameobject_lua;
		m_lua["Scene"]["add_gameobject"] = &Scene::add_gameobject;
		m_lua["Scene"]["build"] = &Scene::build;
	}

	{ // Globals
		m_lua.new_usertype<State>("State",
														"dt", &State::dt,
														"window", &State::window,
														"engine", &State::engine,
														"scene", &State::scene);

		m_lua["state"] = &JenjinState;
		m_lua["s"] = &JenjinState;
	}

	// Bind GLFW key input function and key enum
	m_lua.set_function("is_key_pressed", [](int key) {
		return glfwGetKey(JenjinState.window->getWindow(), key) == GLFW_PRESS;
	});

	m_lua.set_function("get_mouse_position", []() {
		glm::vec2 pos = JenjinState.mouse_pos;
		pos.y = JenjinState.window_size.y - pos.y;
		return pos;
	});

	m_lua["create_quad"] = &Jenjin::shapes::create_quad;

	BIND_ENUM(GLFW_KEY_W);
	BIND_ENUM(GLFW_KEY_S);
	BIND_ENUM(GLFW_KEY_A);
	BIND_ENUM(GLFW_KEY_D);

	BIND_ENUM(GLFW_KEY_SPACE);

	spdlog::debug("Bound keys to lua");

	// nilobj() returns lua nil
	m_lua.set_function("nilgobj", []() {
		return sol::nil;
	});

	m_lua["screen_to_ndc"] = &screen_to_ndc;
}

void Lua::script(const std::string& script) {
	m_lua.script(script);
}

void Lua::script_file(const std::string& path) {
	m_lua.script_file(path);
}
