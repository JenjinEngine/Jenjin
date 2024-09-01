#include "gameobject.h"

#include <glm/glm.hpp>

using namespace Jenjin;

GameObject::GameObject(MeshData meshdata) : meshdata(meshdata) {
	this->id = -1;
}
