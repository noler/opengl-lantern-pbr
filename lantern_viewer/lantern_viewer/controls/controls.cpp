#include "controls.h"
#include "trackball.h"
#include <algorithm>
#include "../utils.h"
#include "../mesh/mesh.h"
#include <imgui.h>
#include "../imgui/lib/imgui_impl_glfw_gl3.h"

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
	
	ctx->camera.position = glm::vec3(ctx->camera.position.x * ctx->camera.camera_projection.zoomFactor, ctx->camera.position.y, ctx->camera.position.z);
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
	ctx->aspect = float(ctx->global_settings.width) / float(ctx->global_settings.height);
	glViewport(0, 0, width, height);
	updateCamera(*ctx);
}

void cursorPosCallback(GLFWwindow* window, double x, double y)
{
	Context* ctx = static_cast<Context *>(glfwGetWindowUserPointer(window));
	moveTrackball(ctx, x, y);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	// Forward event to GUI
	ImGui_ImplGlfwGL3_MouseButtonCallback(window, button, action, mods);
	if (ImGui::GetIO().WantCaptureMouse) { return; }  // Skip other handling

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

