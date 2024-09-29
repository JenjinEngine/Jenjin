#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Jenjin {
struct MeshReference {
	int baseVertex;
	int baseIndex;
	int indexCount;
	int vertexCount;
};

struct Vertex {
	glm::vec3 position;
	glm::vec3 color;
	glm::vec2 texCoord;
};

struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<GLuint> indices;
};
}
