#include "jenjin/helpers.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <spdlog/spdlog.h>

namespace Jenjin::Helpers {
GLFWwindow* CreateWindow(int width, int height, const char* title) {
	if (!glfwInit()) {
		return nullptr;
	}

	GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);

	if (!window) {
		glfwTerminate();
		return nullptr;
	}

	glfwMakeContextCurrent(window);
	return window;
}

void CheckWindow(GLFWwindow* window) {
	if (!window) {
		const char* error;
		int code = glfwGetError(&error);
		spdlog::error("Failed to create window: {} ({})", error, code);
		exit(EXIT_FAILURE);
	}
}

void InitiateImGui(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

Mesh CreateQuad(int width, int height) {
	Mesh mesh;

	mesh.vertices = {
		Vertex{{-width / 2, -height / 2, 0.0f}, {0.0f, 0.0f}},
		Vertex{{width / 2, -height / 2, 0.0f}, {1.0f, 0.0f}},
		Vertex{{width / 2, height / 2, 0.0f}, {1.0f, 1.0f}},
		Vertex{{-width / 2, height / 2, 0.0f}, {0.0f, 1.0f}},
	};

	mesh.indices = {0, 1, 2, 2, 3, 0};
	return mesh;
}
}
