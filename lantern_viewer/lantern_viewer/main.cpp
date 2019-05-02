#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "assimp/assimp.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <specstrings.h>
#include <string>

#include <Windows.h>
#include <Pathcch.h>
#include "utils.h"

#include "data_types/context.h"
#include "data_types/trackball.h"

#include "mesh/mesh.h"

#include "imgui/menu.h"


#include <boost/algorithm/string/replace.hpp>


std::string getExecPath()
{
	char result[MAX_PATH];
	std::string path = std::string(result, GetModuleFileNameA(nullptr, result, MAX_PATH));
	path = path.substr(0, path.find_last_of("\\/"));
	return boost::replace_all_copy(path, "\\", "/");
}

void display(Context& ctx)
{
	glClearColor(
		ctx.global_settings.window_background_color.r,
		ctx.global_settings.window_background_color.g,
		ctx.global_settings.window_background_color.b,
		ctx.global_settings.window_background_color.a
	);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST); // ensures that polygons overlap correctly
	glEnable(GL_MULTISAMPLE);
	drawMesh(ctx, ctx.shader_program, ctx.meshVAO);

	displayImGui();
}

void initializeTrackball(Context& ctx)
{
	double radius = double(std::min(ctx.global_settings.width, ctx.global_settings.height)) / 2.0;
	ctx.trackball.radius = radius;
	glm::vec2 center = glm::vec2(ctx.global_settings.width, ctx.global_settings.height) / 2.0f;
	ctx.trackball.center = center;
}

const double zoomStartValue = 1.0;

void init(Context& ctx)
{
	ctx.shader_program = loadShaderProgram(getExecPath() + "/shaders/mesh.vert",
	                                       getExecPath() + "/shaders/mesh.frag");

	ModelManager manager;
	manager.loadModel(getExecPath() + "/models/lantern/lantern_obj.obj");
	Mesh myMesh = manager.getMesh();
	ctx.mesh = myMesh;
	createMeshVAO(ctx, ctx.mesh, &ctx.meshVAO);

	initializeTrackball(ctx);
	ctx.zoomFactor = zoomStartValue;
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

void moveTrackball(Context* ctx, int x, int y)
{
	if (ctx->trackball.tracking)
	{
		trackballMove(ctx->trackball, glm::vec2(x, y));
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
const double minZoomFactor = 0.1;

// maxZoom must be < 2
const double maxZoomFactor = 1.9;
// zoomStepFactor must be > 1
double zoomStepFactor = 1.1;

void scrollCallback(GLFWwindow* window, double x_offset, double y_offset)
{
	Context *ctx = static_cast<Context *>(glfwGetWindowUserPointer(window));
	if(y_offset < 0)
	{
		ctx->zoomFactor *= zoomStepFactor;
	} else if (y_offset > 0)
	{
		ctx->zoomFactor /= zoomStepFactor;
	}
	ctx->zoomFactor = glm::clamp(ctx->zoomFactor, minZoomFactor, maxZoomFactor);
}

void reloadShaders(Context* ctx)
{
	glDeleteProgram(ctx->shader_program);
	ctx->shader_program = loadShaderProgram(getExecPath() + "/shaders/mesh.vert",
	                                        getExecPath() + "/shaders/mesh.frag");
}

void keyCallback(GLFWwindow* window, int key, int /*scancode*/, int action, int /*mods*/)
{
	Context* ctx = static_cast<Context *>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_R && action == GLFW_PRESS)
	{
		reloadShaders(ctx);
	}
}


int main(void)
{
	Context ctx;

	// Create a GLFW window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_SAMPLES, 8);

	ctx.aspect = float(ctx.global_settings.width) / float(ctx.global_settings.height);
	ctx.window = glfwCreateWindow(ctx.global_settings.width, ctx.global_settings.height, "Lantern Viewer", nullptr,
	                              nullptr);

	glfwMakeContextCurrent(ctx.window);
	glfwSetWindowUserPointer(ctx.window, &ctx);

	glfwSetKeyCallback(ctx.window, keyCallback);
	glfwSetMouseButtonCallback(ctx.window, mouseButtonCallback);
	glfwSetCursorPosCallback(ctx.window, cursorPosCallback);
	glfwSetFramebufferSizeCallback(ctx.window, resizeCallback);
	glfwSetScrollCallback(ctx.window, scrollCallback);

	// Load OpenGL functions
	glewExperimental = true;
	GLenum status = glewInit();
	if (status != GLEW_OK)
	{
		std::cerr << "Error: " << glewGetErrorString(status) << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
	// Initialize GUI
	initImGui(ctx);

	// Initialize rendering
	glGenVertexArrays(1, &ctx.defaultVAO);
	glBindVertexArray(ctx.defaultVAO);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	init(ctx);

	// Start rendering loop
	while (!glfwWindowShouldClose(ctx.window))
	{
		glfwPollEvents();
		ctx.elapsed_time = glfwGetTime();
		display(ctx);
		glfwSwapBuffers(ctx.window);
	}

	// Shutdown
	glfwDestroyWindow(ctx.window);
	glfwTerminate();
	std::exit(EXIT_SUCCESS);
}
