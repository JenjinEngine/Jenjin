#include "editor.h"
#include "engine.h"
#include "utils.h"

#include <spdlog/spdlog.h>

int main(void) {
	spdlog::set_level(spdlog::level::trace);

	JenjinEditor::Window window(800, 600, "Jenjin Editor");
	JenjinEditor::Editor editor;

	Jenjin::Engine = new Jenjin::Engine_t();
	Jenjin::Engine->set_window(window.context);
	Jenjin::Engine->running = true;

	auto scene = new Jenjin::Scene("test.jenscene");
	Jenjin::Engine->add_scene(scene, true);

	window.render([&](GLFWwindow* context) {
		editor.show_all(Jenjin::Engine->active_scene);
	});

	window.launch();

	return 0;
}
