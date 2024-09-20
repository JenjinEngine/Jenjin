#ifndef JENJIN_LUA_MANAGER_H
#define JENJIN_LUA_MANAGER_H

#include "sol/forward.hpp"
#include "sol/state.hpp"
#include <sol/sol.hpp>

#include <string>
#include <vector>

namespace Jenjin {
class LuaManager {
public:
	LuaManager();

	void script(std::string code);
	void script_file(std::string file);

	void ready();
	void update();

	void update_ptrs();

private:
	sol::state state;

	std::vector<sol::protected_function> m_update_functions = {};
	std::vector<sol::protected_function> m_ready_functions = {};

	void bindings();
};
}

#endif // JENJIN_LUA_MANAGER_H
