#pragma once

#include "sol/forward.hpp"
#include "sol/state.hpp"
#include <sol/sol.hpp>

#include <string>

typedef sol::protected_function pf;

namespace Jenjin {
class LuaManager {
public:
	LuaManager();

	void script(std::string code);
	void script_file(std::string file);

	void reload_files();

	void ready();
	void update();

	void update_ptrs();

private:
	sol::state state;

	std::unordered_map<std::string, std::pair<pf, pf>> m_functions = {};

	void bindings();
};
}
