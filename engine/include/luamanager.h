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
	void script_dir(std::string dirPath, bool deleteOthers = false);

	void reload_files();

	void ready();
	void update();

	void update_ptrs();

	void load_files();

private:
	sol::state state;

	std::unordered_map<std::string, std::pair<pf, pf>> m_functions = {};

	void bindings();
};
}
