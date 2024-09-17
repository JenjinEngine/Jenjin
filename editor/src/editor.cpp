#include "editor.h"

#include <gtc/type_ptr.hpp>
#include <spdlog/spdlog.h>

#include <filesystem>
#include <fstream>

void header(std::string title) {
	ImGui::Spacing();
	ImGui::Text("%s", title.c_str());
	ImGui::Separator();
	ImGui::Spacing();
}

void save(Jenjin::Scene* scene) {
	std::ofstream ose("editor.jenscene");
	std::ofstream ost("test.jenscene");
	scene->save(ose);
	scene->save(ost);
}

std::string getFilename(const std::string& fullPath) {
    auto lastSlashPos = fullPath.find_last_of('/');

    if (lastSlashPos == std::string::npos) {
        return fullPath;
    }

    return fullPath.substr(lastSlashPos + 1);
}

void texture_picker(Jenjin::Scene* scene, std::shared_ptr<Jenjin::GameObject>& gobj) {
	ImGui::Text("Texture %s", gobj->texture_path.empty() ? "None" : getFilename(gobj->texture_path).c_str());

	ImGui::SameLine();
	if (ImGui::SmallButton("Change")) {
		auto* path = new char[256];
		ImGui::OpenPopup("Texture picker");
	}
	ImGui::SameLine();
	if (ImGui::SmallButton("Remove")) {
		gobj->remove_texture();
	}

	if (ImGui::BeginPopup("Texture picker")) {
		header("Texture picker");

		const char* path = "test/assets/";
		for (auto& p : std::filesystem::directory_iterator(path)) {
			const std::string ext = p.path().extension().string();
			if (ext != ".png" && ext != ".jpg" && ext != ".jpeg")
				continue;

			if (ImGui::Selectable(p.path().filename().string().c_str())) {
				gobj->set_texture(p.path().string(), ext == ".png");
			}
		}

		ImGui::EndPopup();
	}
}
