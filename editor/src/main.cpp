#include "engine.h"
#include "scene.h"

#include "window.h"
#include "editor.h"

#include <spdlog/spdlog.h>

#include <imgui_internal.h>
#include <imgui.h>

#ifdef JENJIN_HEADLESS
int main(void) {
	Window window;

	window.render([&](Jenjin::Scene* scene) {
		jenjin_menubar(scene);

		int dockspace_id = jenjin_dockspace();

		ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		jenjin_explorer(scene);
		ImGui::SetNextWindowDockID(dockspace_id, ImGuiCond_FirstUseEver);
		jenjin_viewport();

		ImGui::DockBuilderFinish(dockspace_id);

		ImGui::End();
	});

	Jenjin::Engine = new Jenjin::Engine_t();
	Jenjin::Engine->set_window(window.context);
	Jenjin::Scene scene("test.jenscene");
	Jenjin::Engine->add_scene(&scene, true);

	Jenjin::Engine->running = true;
	window.launch(800, 600);
}
#else
int main(void) {
	spdlog::error("Editor wasn't built with JENJIN_HEADLESS defined");
	return 1;
}
#endif
