#include "utils.h"
#include "imgui.h"

#include <spdlog/spdlog.h>

#include <cstdlib>

using namespace JenjinEditor;

Window::Window(int width, int height, const char* title) {
	glfwSetErrorCallback([](int error, const char* description) {
		spdlog::error("Jenjin editor error callback...");
		spdlog::error("GLFW Error {}: {}", error, description);
	});

	if (!glfwInit()) {
		spdlog::error("Failed to initialize GLFW");
		exit(EXIT_FAILURE);
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	context = glfwCreateWindow(width, height, title, NULL, NULL);

	if (!context) {
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	glfwMakeContextCurrent(context);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		spdlog::error("Failed to initialize OpenGL context");
		exit(EXIT_FAILURE);
	}

	glfwSwapInterval(1);

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	ImGuiStyle& style = ImGui::GetStyle();
	for (int i = 0; i < ImGuiCol_COUNT; i++) {
		static ImVec4* colors = style.Colors;
		float h, s, v; ImGui::ColorConvertRGBtoHSV(colors[i].x, colors[i].y, colors[i].z, h, s, v);
		h = 0; // red
		float r, g, b; ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
		colors[i] = ImVec4(r, g, b, colors[i].w);
	}

	// Make everything round (sharp == harsh in design i think?)
	style.FrameRounding = 4.0f;
	style.WindowRounding = 4.0f;

	// Disable window frame
	style.WindowBorderSize = 0.0f;

	io.Fonts->AddFontFromFileTTF("engine/resources/fonts/Roboto-Medium.ttf", 16.0f);

	ImGui_ImplGlfw_InitForOpenGL(context, true);
	ImGui_ImplOpenGL3_Init("#version 460");
}

void Window::launch() {
	while (!glfwWindowShouldClose(context)) {
		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		m_renderCallback(context);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(context);
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(context);
	glfwTerminate();
}
