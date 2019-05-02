#include "trackball.h"

#include <cmath>
#include <algorithm>

// Helper functions
glm::vec3 mapMousePointToUnitSphere(glm::vec2 point, double radius, glm::vec2 center)
{
	float x = point[0] - center[0];
	float y = -point[1] + center[1];
	float z = 0.0f;
	if (x * x + y * y < radius * radius / 2.0f)
	{
		z = std::sqrt(radius * radius - (x * x + y * y));
	}
	else
	{
		z = (radius * radius / 2.0f) / std::sqrt(x * x + y * y);
	}
	return glm::normalize(glm::vec3(x, y, z));
}

// Start trackball tracking
void trackballStartTracking(Trackball& trackball, glm::vec2 point)
{
	trackball.vStart = mapMousePointToUnitSphere(point, trackball.radius, trackball.center);
	trackball.qStart = glm::quat(trackball.qCurrent);
	trackball.tracking = true;
}

// Stop trackball tracking
void trackballStopTracking(Trackball& trackball)
{
	trackball.tracking = false;
}

// Rotate trackball from, e.g., mouse movement
void trackballMove(Trackball& trackball, glm::vec2 point)
{
	glm::vec3 vCurrent = mapMousePointToUnitSphere(point, trackball.radius, trackball.center);
	glm::vec3 rotationAxis = glm::cross(trackball.vStart, vCurrent);
	float dotProduct = std::max(std::min(glm::dot(trackball.vStart, vCurrent), 1.0f), -1.0f);
	float rotationAngle = std::acos(dotProduct);
	float eps = 0.01f;
	if (rotationAngle < eps)
	{
		trackball.qCurrent = glm::quat(trackball.qStart);
	}
	else
	{
		// Note: here we provide rotationAngle in radians. Older versions
		// of GLM (0.9.3 or earlier) require the angle in degrees.
		glm::quat q = glm::angleAxis(rotationAngle, rotationAxis);
		q = glm::normalize(q);
		trackball.qCurrent = glm::normalize(glm::cross(q, trackball.qStart));
	}
}

// Get trackball orientation in matrix form
glm::mat4 trackballGetRotationMatrix(Trackball &trackball)
{
	return glm::mat4_cast(trackball.qCurrent);
}