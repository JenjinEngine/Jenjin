#ifndef JENJIN_SCRIPTMANAGER_H
#define JENJIN_SCRIPTMANAGER_H

#include "lua.h"
#include <sol/forward.hpp>
#include <string>
#include <unordered_map>

struct ScriptFunctions {
	sol::protected_function update;
	sol::protected_function ready;
};

namespace Jenjin {
class ScriptManager {
public:
	ScriptManager() = default;

	void add_script(const std::string& path);
	void add_directory(const std::string& path);

	void fill_in_glfw();

	void reload_scripts();

	void ready();
	void update();

private:
	std::unordered_map<std::string, ScriptFunctions> m_script_functions;

	Lua lua;
};
}

#endif // JENJIN_SCRIPTMANAGER_H
