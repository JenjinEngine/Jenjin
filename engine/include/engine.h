#ifndef JENJIN_ENGINE_H
#define JENJIN_ENGINE_H

#include "camera.h"
#include "scriptmanager.h"
#include "window.h"
#include "scene.h"

#include <functional>
#include <glm/fwd.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>

#include <imgui.h>

namespace Jenjin {
class Engine {
public:
	Engine();
	~Engine();

	void check_version(void);

	void launch(int width, int height, const char* title);

	void add_scene(Scene* scene);
	bool activate_scene(int index);

	void set_resized_callback(GLFWwindowsizefun callback);
	void set_key_callback(GLFWkeyfun callback);
	void set_mouse_callback(GLFWcursorposfun callback);

	void set_render_callback(std::function<void(Engine*, GLFWwindow*)> callback);

#ifndef NDEBUG
	// NOTE: This function only exists in debug mode
	void set_debug_callback(std::function<void(Engine*, GLFWwindow*)> callback) {
		m_debug_callback = callback;
	}
#endif

private:
	ScriptManager* m_script_manager = nullptr;

	std::function<void(Engine*, GLFWwindow*)> m_render_callback;

#ifndef NDEBUG
	std::function<void(Engine*, GLFWwindow*)> m_debug_callback = [](Engine* engine, GLFWwindow* window) {
		ImGui::SetNextWindowSize(ImVec2(600, 300), ImGuiCond_FirstUseEver);
		ImGui::Begin("Debug");
		if (ImGui::CollapsingHeader("Stats")) {
			ImGui::Indent();
			ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
			ImGui::Text("GameObjects: %lu", engine->m_active_scene->m_gameobjects.size());

			ImGui::Text("Debug window: (%.1f, %.1f)", ImGui::GetWindowWidth(), ImGui::GetWindowHeight());

			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("Camera")) {
			ImGui::Indent();
			Camera* camera = &engine->m_active_scene->m_camera;
			std::string position = fmt::format("Position: ({}, {}, {})", camera->position.x, camera->position.y, camera->position.z);
			ImGui::Text("%s", position.c_str());
			ImGui::SameLine();
			if (ImGui::Button("Reset")) {
				camera->position = glm::vec3(0.0f, 0.0f, 100.0f);
				camera->m_changed_projection = true;
			}

			ImGui::Text("Zoom: %.1f", camera->zoom);
			ImGui::SameLine();
			if (ImGui::Button("Reset##zoom")) {
				camera->zoom = 1.0f;
				camera->m_changed_projection = true;
			}

			ImGui::Unindent();
		}

		if (ImGui::CollapsingHeader("GameObjects")) {
			ImGui::Indent();

			for (GameObject* game_object : engine->m_active_scene->m_gameobjects) {
				ImGui::PushID(game_object->id);
				if (ImGui::CollapsingHeader(game_object->name.c_str())) {
					ImGui::Indent();

					ImGui::InputFloat("x", &game_object->transform.position.x);
					ImGui::SameLine();
					if (ImGui::Button("Reset##x"))
						game_object->transform.position.x = 0.0f;

					ImGui::InputFloat("y", &game_object->transform.position.y);
					ImGui::SameLine();
					if (ImGui::Button("Reset##y"))
						game_object->transform.position.y = 0.0f;

					ImGui::InputFloat3("Colour", glm::value_ptr(game_object->color));

					ImGui::Unindent();
				}
				ImGui::PopID();
			}

			ImGui::Unindent();
		}
		ImGui::End();
	};
#endif

	Window m_window;
	Scene* m_active_scene = nullptr;
	std::vector<Scene*> m_scenes;

	// Used in delta time calculation
	float m_lastFrame = 0.0f;
};
}

#endif // JENJIN_ENGINE_H
