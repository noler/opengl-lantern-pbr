#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/lib/imgui.h"
#include "imgui/lib/imgui_impl_glfw_gl3.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <specstrings.h>
#include <string>

#include <Windows.h>
#include <Pathcch.h>
#include <assimp/Importer.hpp>      // C++ importer interface
#include "utils.h"

#include "mesh/mesh.h"

#include "data_types/context.h"

#include <boost/algorithm/string/replace.hpp>


std::string getExecPath()
{
	char result[MAX_PATH];
	std::string path = std::string(result, GetModuleFileNameA(nullptr, result, MAX_PATH));
	path = path.substr(0, path.find_last_of("\\/"));
	return boost::replace_all_copy(path, "\\", "/");
}

void displayImGui()
{
	bool dummy[] = {true};
	ImGui_ImplGlfwGL3_NewFrame();
	ImGui::Begin("This is a title", dummy);
	ImGui::Text("Magnus is cool");
	ImGui::Checkbox("Check me", dummy);
	ImGui::End();
	ImGui::Render();
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
	drawMesh(ctx, ctx.shader_program, ctx.meshVAO);

	displayImGui();
}

void init(Context& ctx)
{
	ctx.shader_program = loadShaderProgram(getExecPath() + "/shaders/mesh.vert",
	                                getExecPath() + "/shaders/mesh.frag");

	loadMesh(getExecPath() + "/models/bunny.obj", &ctx.mesh);
	createMeshVAO(ctx, ctx.mesh, &ctx.meshVAO);
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

	ctx.aspect = float(ctx.global_settings.width) / float(ctx.global_settings.height);
	ctx.window = glfwCreateWindow(ctx.global_settings.width, ctx.global_settings.height, "Lantern Viewer", nullptr, nullptr);

	glfwMakeContextCurrent(ctx.window);
	glfwSetWindowUserPointer(ctx.window, &ctx);

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
	ImGui_ImplGlfwGL3_Init(ctx.window, false /*do not install callbacks*/);

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
