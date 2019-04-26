#pragma once
#include <glm/vec4.hpp>

struct GlobalSettings
{
	glm::vec4 window_background_color = glm::vec4(0.2, 0.2, 0.2, 1.0);
	int width = 1100;
	int height = 900;
	float zoom_factor = 1.0f;
};
