#ifndef JENJIN_TRANSFORM_H
#define JENJIN_TRANSFORM_H

#include "glm/glm.hpp"

struct Transform {
	glm::vec3 position;
	float rotation; // NOTE: this could be a vec2 if we want the user to have more control
};

#endif // JENJIN_TRANSFORM_H
