#ifndef JENJIN_LUA_H
#define JENJIN_LUA_H

#include <sol/sol.hpp>
#include <sol/state.hpp>

class Lua {
public:
	Lua();

	void script(const std::string& script);
	void script_file(const std::string& path);

	sol::state* get_lua_state() { return &m_lua; }

private:
	sol::state m_lua;
};

#endif // JENJIN_LUA_H
