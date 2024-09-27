#pragma once

#include <string>
namespace JenjinEditor {
struct State_t {
	std::string projectPath;
	std::string openScenePath;
	std::string liveScenePath;
	std::string scriptsPath;
} extern State;
}

