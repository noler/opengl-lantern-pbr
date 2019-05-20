#pragma once

#include <glm/vec3.hpp>

struct MaterialSettings
{
	int use_albedo_map = 1;
	glm::vec3 albedo_color = glm::vec3(1.0, 1.0, 1.0);

	int use_roughness_map = 1;
	float roughness_value = 0.5;

	int use_metallic_map = 1;
	float metallic_value = 1.0;

	int use_L0 = 1;
	int use_ambient_IBL = 1;

	float normal_map_influence = 1.0;
	
};
