#ifndef JENJIN_MESH_H
#define JENJIN_MESH_H

#include <glm/glm.hpp>

namespace Jenjin {
struct Vertex {
	glm::vec3 position;
	glm::vec2 tex_coords;
};

struct MeshData {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

class Mesh {
public:
	int base_vertex;
	int base_index;
	int vertex_count;
	int index_count;
};
}

#endif // JENJIN_MESH_H
