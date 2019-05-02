#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>


struct Trackball {
	double radius;
	glm::vec2 center;
	bool tracking;
	glm::vec3 vStart;
	glm::quat qStart;
	glm::quat qCurrent;

	Trackball() : radius(1.0),
		center(glm::vec2(0.0f, 0.0f)),
		tracking(false),
		vStart(glm::vec3(0.0f, 0.0f, 1.0f)),
		qStart(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
		qCurrent(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
	{}
};

glm::vec3 mapMousePointToUnitSphere(glm::vec2 point, double radius, glm::vec2 center);

void trackballStartTracking(Trackball& trackball, glm::vec2 point);

void trackballStopTracking(Trackball& trackball);

void trackballMove(Trackball& trackball, glm::vec2 point);

glm::mat4 trackballGetRotationMatrix(Trackball &trackball);