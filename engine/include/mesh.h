#ifndef JENJIN_MESH_H
#define JENJIN_MESH_H

#include <glm.hpp>

namespace Jenjin {
// A vertex is a single point in 3D space. It contains
// a position and texture coordinates.
//
// Other data can be added to the vertex struct to
// support more advanced rendering techniques. For
// example, instead of a colour uniform, a colour
// could be added to the vertex struct.
struct Vertex {
	glm::vec2 position;
	glm::vec2 tex_coords;
};

// A mesh is a collection of vertices and indices that
// define a 3D object.
struct Mesh {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

// A mesh reference refers to the data inside of the main
// buffers where a mesh is stored. This allows for multiple
// meshes to share the same vertex and index buffers.
class MeshReference {
public:
	int base_vertex;
	int base_index;
	int vertex_count;
	int index_count;
};
}

#endif // JENJIN_MESH_H
