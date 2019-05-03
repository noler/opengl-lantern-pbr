#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "assimp/assimp.h"

#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <cstdlib>
#include <specstrings.h>
#include <string>

#include <Windows.h>
#include <Pathcch.h>
#include "utils.h"

#include "data_types/context.h"

#include "mesh/mesh.h"

#include "imgui/menu.h"

#include "controls/controls.h"
#include "controls/trackball.h"

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
	drawMesh(ctx, ctx.shader_program, ctx.meshVAO1);
	drawMesh(ctx, ctx.shader_program, ctx.meshVAO2);

	displayImGui();
}

void init(Context& ctx)
{
	ctx.shader_program = loadShaderProgram(getExecPath() + "/shaders/mesh.vert",
	                                       getExecPath() + "/shaders/mesh.frag");

	ModelManager manager;
	manager.loadModel(getExecPath() + "/models/lantern/lantern_obj.obj");
	std::vector<Mesh> meshes = manager.getMesh();

	ctx.mesh1 = meshes.at(0);
	createMeshVAO(ctx, ctx.mesh1, &ctx.meshVAO1);

	ctx.mesh2 = meshes.at(1);
	createMeshVAO(ctx, ctx.mesh2, &ctx.meshVAO2);

	initializeTrackball(ctx);
	ctx.zoomFactor = zoomStartValue;
}

void reloadShaders(Context *ctx)
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
