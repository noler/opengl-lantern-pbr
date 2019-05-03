#include "controls.h"
#include "trackball.h"
#include <algorithm>
#include "../utils.h"
#include "../mesh/mesh.h"

// zoomStepFactor must be > 1
double zoomStepFactor = 1.1;

void scrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
	Context *ctx = static_cast<Context *>(glfwGetWindowUserPointer(window));
	if (y_offset < 0)
	{
		ctx->camera.camera_projection.zoomFactor *= zoomStepFactor;
	}
	else if (y_offset > 0)
	{
		ctx->camera.camera_projection.zoomFactor /= zoomStepFactor;
	}
	ctx->camera.camera_projection.zoomFactor = glm::clamp(ctx->camera.camera_projection.zoomFactor, minZoomFactor, maxZoomFactor);

	updateCamera(*ctx);
}

void mouseButtonPressed(Context* ctx, int button, int x, int y)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		ctx->trackball.center = glm::vec2(x, y);
		trackballStartTracking(ctx->trackball, glm::vec2(x, y));
	}
}

void mouseButtonReleased(Context* ctx, int button, int x, int y)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT)
	{
		trackballStopTracking(ctx->trackball);
	}
}


void resizeCallback(GLFWwindow* window, int width, int height)
{
	Context* ctx = static_cast<Context *>(glfwGetWindowUserPointer(window));
	ctx->global_settings.width = width;
	ctx->global_settings.height = height;
	ctx->trackball.radius = double(std::min(width, height)) / 2.0;
	ctx->trackball.center = glm::vec2(width, height) / 2.0f;
	glViewport(0, 0, width, height);
}

void cursorPosCallback(GLFWwindow* window, double x, double y)
{
	Context* ctx = static_cast<Context *>(glfwGetWindowUserPointer(window));
	moveTrackball(ctx, x, y);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	double x, y;
	glfwGetCursorPos(window, &x, &y);

	Context* ctx = static_cast<Context *>(glfwGetWindowUserPointer(window));
	if (action == GLFW_PRESS)
	{
		mouseButtonPressed(ctx, button, x, y);
	}
	else
	{
		mouseButtonReleased(ctx, button, x, y);
	}
}

