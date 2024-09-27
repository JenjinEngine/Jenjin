#include "editor.h"
#include "engine.h"
#include "state.h"
#include "utils.h"

#include <spdlog/spdlog.h>
#include <string>

int main(void) {
	spdlog::set_level(spdlog::level::trace);

	JenjinEditor::Window window(800, 600, "Jenjin Editor");
	JenjinEditor::Editor editor;

	Jenjin::Engine = new Jenjin::Engine_t();
	Jenjin::Engine->set_window(window.context);
	Jenjin::Engine->running = true;
	Jenjin::Engine->editor = true;

	auto scene = new Jenjin::Scene("test.jenscene");
	Jenjin::Engine->add_scene(scene, true);

	std::string jendir = get_jendir();

	if (jendir.empty()) {
		spdlog::warn("Couldn't find user directory, defaulting to current directory under `Jendir`... report this issue as this is not the intended behavior");
		jendir = std::string("Jendir");
	}

	ensure_dir(jendir + "/Projects");

	window.render([&](GLFWwindow* context) {
		if (JenjinEditor::State.projectPath.empty()) {
			editor.menu();
			editor.dockspace();
			editor.welcome();
		} else {
			editor.show_all(Jenjin::Engine->active_scene);
		}

		if (!Jenjin::Engine->editor) {
			Jenjin::Engine->active_scene->update();
		}
	});

	window.launch();

	return 0;
}
