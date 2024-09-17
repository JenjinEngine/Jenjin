#include "window.h"
#include "engine.h"

#include <spdlog/spdlog.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

Window::Window() {
	if (!glfwInit()) {
		const char* error;
		int code = glfwGetError(&error);
		spdlog::error("Failed to initialize GLFW: {} ({})", error, code);
	}

	spdlog::debug("Initialized window");

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	context = glfwCreateWindow(1, 1, "Jenjin editor", NULL, NULL);

	if (!context) {
		const char* error;
		int code = glfwGetError(&error);
		spdlog::error("Failed to create window: {} ({})", error, code);
	}

	glfwMakeContextCurrent(context);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		spdlog::error("Failed to initialize GLAD");
	}

	spdlog::debug("Initialized OpenGL");

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui_ImplGlfw_InitForOpenGL(context, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	ImGui::StyleColorsDark();
	/* ImGui::StyleColorsLight(); */

	// load engine/resources/fonts/Roboto-Medium.ttf
	io.Fonts->AddFontFromFileTTF("engine/resources/fonts/Roboto-Medium.ttf", 16.0f);
	io.FontDefault = io.Fonts->Fonts.back();

	ImGuiStyle& style = ImGui::GetStyle();
	style.TabRounding = 2.0f;
	style.ChildRounding = 4.0f;
	style.FrameRounding = 4.0f;
	style.PopupRounding = 4.0f;
	style.WindowRounding = 4.0f;

	spdlog::debug("Initialized ImGui version {}", IMGUI_VERSION);
}

void Window::render(std::function<void (Jenjin::Scene*)> render_callback) {
	m_render_callback = render_callback;
}

void Window::launch(int width, int height) {
	glfwSetWindowSize(context, width, height);

	while (!glfwWindowShouldClose(context)) {
		glClearColor(0.07f, 0.13f, 0.17f, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glfwPollEvents();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (m_render_callback)
			m_render_callback(Jenjin::Engine->active_scene);

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(context);
	}
}
