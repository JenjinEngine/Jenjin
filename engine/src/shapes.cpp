#include "shapes.h"

using namespace Jenjin;

MeshData Jenjin::shapes::create_quad(float width, float height) {
	MeshData data = {};

	data.vertices = {
		// Position, Texture coordinates
		{ glm::vec3(-width / 2.0f, -height / 2.0f, 0.0f), glm::vec2(0.0f, 0.0f) },
		{ glm::vec3(width / 2.0f, -height / 2.0f, 0.0f), glm::vec2(1.0f, 0.0f) },
		{ glm::vec3(width / 2.0f, height / 2.0f, 0.0f), glm::vec2(1.0f, 1.0f) },
		{ glm::vec3(-width / 2.0f, height / 2.0f, 0.0f), glm::vec2(0.0f, 1.0f) }
	};

	data.indices = {
		0, 1, 2,
		2, 3, 0
	};

	return data;
}

