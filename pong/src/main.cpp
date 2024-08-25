#include "engine.h"
#include "mesh.h"
#include "scene.h"

#include "shapes.h"

int main(void) {
	// Setup engine
	Jenjin::Engine engine;

	// Create paddle meshes
	Jenjin::MeshData leftPaddle = Jenjin::shapes::create_quad(-0.9f, -0.2f, 0.1f, 0.4f);
	Jenjin::MeshData rightPaddle = Jenjin::shapes::create_quad(0.8f, -0.2f, 0.1f, 0.4f);

	// Create a scene with the meshes
	Jenjin::Scene scene;
	scene.add_mesh(leftPaddle);
	scene.add_mesh(rightPaddle);

	// Add scene to engine
	engine.add_scene(scene);
	engine.activate_scene(0);

	// Launch engine
	engine.launch(800, 600, "Pong");
}
