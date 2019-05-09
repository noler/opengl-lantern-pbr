#pragma once

#include <glm/vec3.hpp>

struct MaterialSettings
{
	int use_albedo_map = 1;
	glm::vec3 albedo_color = glm::vec3(1.0, 1.0, 1.0);
};
