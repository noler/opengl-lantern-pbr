#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include "../data_types/context.h"

void initializeTrackball(Context& ctx);

glm::vec3 mapMousePointToUnitSphere(glm::vec2 point, double radius, glm::vec2 center);

void trackballStartTracking(Trackball& trackball, glm::vec2 point);

void trackballStopTracking(Trackball& trackball);

glm::mat4 trackballGetRotationMatrix(Trackball &trackball);

void trackballMove(Trackball& trackball, glm::vec2 point);

void moveTrackball(Context* ctx, int x, int y);