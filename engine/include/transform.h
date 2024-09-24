#pragma once

#include <glm.hpp>

// Stores position, rotation, and scale of a game object
// in 2D space
struct Transform {
	glm::vec2 scale = glm::vec2(1.0f, 1.0f);
	glm::vec2 position = glm::vec2(0.0f, 0.0f);
	float rotation = 0.0f;
};
