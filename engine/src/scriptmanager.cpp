#include "scriptmanager.h"

#include <fstream>
#include <filesystem>

#include <sol/forward.hpp>
#include <spdlog/spdlog.h>

#include <fmt/color.h>

using namespace Jenjin;

void ScriptManager::add_script(const std::string& path) {
	std::fstream file(path, std::ios::in);
	if (!file.is_open()) {
		spdlog::error("Could not open lua script file: {}", path);
		return;
	}
	file.close();

	lua.script_file(path);
	sol::function update = lua.get_lua_state()->get<sol::function>("update");
	sol::function ready = lua.get_lua_state()->get<sol::function>("ready");
	this->m_script_functions[path] = {update, ready};
	lua.get_lua_state()->stack_clear();
}

void ScriptManager::add_directory(const std::string& path) {
	for (const auto& entry : std::filesystem::directory_iterator(path)) {
		if (entry.is_regular_file() && entry.path().extension().string() == ".lua") {
			this->add_script(entry.path().string());
		}
	}
}

void ScriptManager::ready() {
	for (auto& [path, funcs] : m_script_functions) {
		sol::protected_function_result result = funcs.ready();
		if (!result.valid()) {
			sol::error err = result;
			spdlog::error("[{}] {}\n", fmt::format(fmt::fg(fmt::color::pale_violet_red), "lua"), err.what());
		}
	}
}

void ScriptManager::update() {
	for (auto& [path, funcs] : m_script_functions) {
		sol::protected_function_result result = funcs.update();
		if (!result.valid()) {
			sol::error err = result;
			spdlog::error("[{}] {}\n", fmt::format(fmt::fg(fmt::color::pale_violet_red), "lua"), err.what());
		}
	}
}

void ScriptManager::fill_in_glfw() {
	lua.fill_in_glfw();
}

void ScriptManager::reload_scripts() {
	for (auto& [path, funcs] : m_script_functions) {
		lua.get_lua_state()->stack_clear();
		this->add_script(path);
	}
}
