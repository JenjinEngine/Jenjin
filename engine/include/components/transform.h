#ifndef JENJIN_TRANSFORM_H
#define JENJIN_TRANSFORM_H

#include "glm/glm.hpp"

struct Transform {
	glm::vec2 position;
	float rotation; // NOTE: this could be a vec2 if we want the user to have more control
	float scale = 1.0f;
	float z_index = 0.0f;
};

#endif // JENJIN_TRANSFORM_H
