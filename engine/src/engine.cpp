#include <any>
#include <spdlog/spdlog.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <fstream>
#include <ostream>

#include "engine.h"
#include "scene.h"

using namespace Jenjin;

/* const char VERSION[] = "0.0.1"; */
#define VERSION "0.0.1"
const char* HEADER = "Jenjin " VERSION;

Engine_t Jenjin::Engine;

Engine_t::Engine_t() {
// Enable debug logging if in debug mode
#ifndef NDEBUG
	spdlog::set_level(spdlog::level::trace);
#endif

	spdlog::debug("Initializing Jenjin {}", VERSION);

	// Intialize GLFW, logging any errors
	if (!glfwInit()) {
		const char* error;
		int code = glfwGetError(&error);
		spdlog::error("Failed to initialize GLFW: {} ({})", error, code);
	}

	// Set OpenGL version to 4.6 Core Profile
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Enable forward compatibility on macOS
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create a hidden window to initialize OpenGL context and load GLAD
	// This is to easily construct types throughout engine initialization
	// that require an OpenGL context
	window = glfwCreateWindow(1, 1, "Jenjin", NULL, NULL);

	// Check if window creation failed
	if (!window) {
		const char* error;
		int code = glfwGetError(&error);
		spdlog::error("Failed to create GLFW window: {} ({})", error, code);
		glfwTerminate();

		throw std::runtime_error("Failed to create GLFW window");
	}

	// Make the window's context current
	glfwMakeContextCurrent(window);

	// Load OpenGL functions using GLAD
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		spdlog::error("Failed to initialize GLAD");
		glfwTerminate();

		throw std::runtime_error("Failed to initialize GLAD");
	}

	spdlog::debug("OpenGL Version: {}", (const char*)glGetString(GL_VERSION));
	spdlog::debug("GLSL Version: {}", (const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));

	spdlog::debug("Vendor: {}", (const char*)glGetString(GL_VENDOR));

	// Set escape key to close window
	glfwSetKeyCallback(window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
			glfwSetWindowShouldClose(window, GLFW_TRUE);
		}

		// key to toggle wireframe
		static bool wireframe = false;
		if (key == GLFW_KEY_F1 && action == GLFW_PRESS) {
			wireframe = !wireframe;
			glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
		}

		if (key == GLFW_KEY_F2 && action == GLFW_PRESS) {
			std::ofstream os("test.jenscene");
			Engine.active_scene->save(os);
		}

		if (key == GLFW_KEY_F3 && action == GLFW_PRESS) {
			std::ifstream is("test.jenscene");
			spdlog::debug("Loading scene from file");

			/* auto scene = new Jenjin::Scene({test1, test2}); */
			auto scene = new Jenjin::Scene();
			scene->load(is);

			Engine.add_scene(scene, true);
			spdlog::debug("Scene loaded from file");
		}
	});

#ifndef NDEBUG
	IMGUI_CHECKVERSION();
	spdlog::debug("ImGui Version: {}", IMGUI_VERSION);

	ImGui::CreateContext();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	io.FontDefault = io.Fonts->AddFontFromFileTTF("engine/resources/fonts/Roboto-Medium.ttf", 16.0f);
#endif
}

void Engine_t::add_scene(Scene* scene, bool active) {
	m_scenes.push_back(scene);

	if (active) {
		active_scene = scene;

		// Resize the scene
		int width, height;
		glfwGetWindowSize(window, &width, &height);
		active_scene->resize(window, width, height);
	}
}

void Engine_t::activate_scene(unsigned int index) {
	if (index < 0 || index >= m_scenes.size()) {
		spdlog::error("Invalid scene index: {}", index);
		return;
	}

	active_scene = m_scenes[index];

	// Resize the scene
	int width, height;
	glfwGetWindowSize(window, &width, &height);
	active_scene->resize(window, width, height);
}

void Engine_t::launch(int width, int height, const char* title) {
	// Build all the scenes
	for (auto& scene : m_scenes)
	scene->build();

	// Setup the window to be the desired size and title
	// instead of the hidden window
	glfwSetWindowSize(window, width, height);
	glfwSetWindowTitle(window, title);

	// Tell OpenGL to draw to the entire window
	glViewport(0, 0, width, height);

	glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
		Engine.active_scene->resize(window, width, height);
	});

	this->running = true;

	// Main loop
	glClearColor(0.07f, 0.13f, 0.17f, 1.0);
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT);

		// Render the active scene
		if (active_scene) {
			active_scene->render();
		} else {
			spdlog::warn("No active scene");
		}

#ifndef NDEBUG
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		// Render ImGui windows
		ImGui::Begin(HEADER);
		ImGui::Text("FPS: %.0f", ImGui::GetIO().Framerate);
		ImGui::Text("Scene count: %d", (int)m_scenes.size());
		ImGui::Text("Frame time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
		ImGui::End();

		this->active_scene->debug_menu();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
#endif

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	spdlog::debug("Deinitializing Jenjin {}", VERSION);

	for (auto& scene : m_scenes)
	delete scene;

	spdlog::debug("Terminating GLFW");
	glfwTerminate();

	/* spdlog::debug("Deinitializing Jenjin {}", VERSION); */

	/* spdlog::debug("Terminating GLFW"); */
	/* glfwTerminate(); */
}
