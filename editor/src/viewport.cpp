#include "engine.h"
#include "editor.h"

#include <imgui.h>

void jenjin_viewport() {
#ifdef JENJIN_HEADLESS
	ImGui::Begin("Viewport");
	ImVec2 size = ImGui::GetContentRegionAvail();
	Jenjin::Engine->render_into_imgui(size.x, size.y);
	ImGui::GetWindowDrawList()->AddImage(
		(void*)(intptr_t)Jenjin::Engine->framebuffer.texture,
		ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(ImGui::GetCursorScreenPos().x + size.x,
				 ImGui::GetCursorScreenPos().y + size.y),
		ImVec2(0, 1),
		ImVec2(1, 0)
	);
	ImGui::End();
#endif
}
