#ifndef JENJIN_SCENE_H
#define JENJIN_SCENE_H

#include "gameobject.h"
#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"

#include <functional>
#include <ostream>
#include <vector>
#include <memory>

namespace Jenjin {
// A scene is a collection of game objects
// that are updated and rendered together
// (e.g. scripting, rendering).
//
// It also manages loading and unloading of
// game objects and their resources.
class Scene {
public:
	// RAII pattern
	Scene();
	Scene(std::vector<std::shared_ptr<GameObject>> game_objects);
	~Scene();

	// Used to correct aspect ratio on resize
	void resize(GLFWwindow* window, int width, int height);

	// Used to rebuild the scene buffers
	void build();

	// Updaters
	void update();
	void render();

	// Debugging
	void debug_menu(bool separate_window = true);

	// Adders
	void add_gameobject(std::shared_ptr<GameObject> game_object);
	void add_gameobjects(std::vector<std::shared_ptr<GameObject>> game_objects);

	std::shared_ptr<GameObject> get_gameobject(std::string name);

	// Texture management
	void load_gameobject_texture(GameObject* game_object);

	// Callback management
	void set_update_callback(std::function<void(Scene*)> callback);
	void set_render_callback(std::function<void(Scene*)> callback);
	void set_resize_callback(std::function<void(Scene*, GLFWwindow*, int, int)> callback);

	// Saving/loading (take in ostream/istream)
	void save(std::ostream &os);
	void load(std::istream &is);

	// Game object pointers
	std::vector<std::shared_ptr<GameObject>> m_game_objects;

private:
	// Game object pointers
	std::vector<MeshReference> m_mesh_references;

	// Texture storage (follows same pattern as mesh references in
	// terms of GameObject access)
	std::unordered_map<std::string, std::shared_ptr<Texture>> m_textures;

	// OpenGL buffers
	unsigned int m_vao, m_vbo, m_ebo = 0;

	// Callbacks
	std::function<void(Scene*)> m_update_callback;
	std::function<void(Scene*)> m_render_callback;
	std::function<void(Scene*, GLFWwindow*, int, int)> m_resize_callback;

	// Default resources
	Shader m_default_shader = Shader("engine/shaders/vshader.glsl", "engine/shaders/fshader.glsl");
	Camera m_default_camera;
};
}

#endif
