#include "state.h"
#include "utils.h"

glm::vec2 screen_to_ndc(const glm::vec2& screen_pos) {
	glm::vec2 inv_window_size = 2.0f / JenjinState.window_size;
	glm::vec2 ndc_pos = (screen_pos * inv_window_size) - glm::vec2(1.0f);
	glm::vec2 camera_position = JenjinState.camera->position;
	ndc_pos = ndc_pos * JenjinState.camera->zoom + camera_position;
	return ndc_pos;
}

