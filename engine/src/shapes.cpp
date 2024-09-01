#include "shapes.h"

using namespace Jenjin;

MeshData Jenjin::shapes::create_quad(float width, float height) {
	MeshData data = {};

	// before: when we weren't using local space
	/* data.vertices = { */
	/* 	// Position, Texture coordinates */
	/* 	{ glm::vec3(x, y, 0.0f), glm::vec2(0.0f, 0.0f) }, */
	/* 	{ glm::vec3(x + width, y, 0.0f), glm::vec2(1.0f, 0.0f) }, */
	/* 	{ glm::vec3(x + width, y + height, 0.0f), glm::vec2(1.0f, 1.0f) }, */
	/* 	{ glm::vec3(x, y + height, 0.0f), glm::vec2(0.0f, 1.0f) } */
	/* }; */

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

