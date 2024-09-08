#include "utils.h"
#include <glm/glm.hpp>

glm::vec2 screen_to_ndc(glm::vec2 screen_position, glm::vec2 window_size) {
	glm::vec2 normalized_screen_position = screen_position / window_size;
	return normalized_screen_position;
}
