// Assignment 3, Part 1 and 2
//
// Modify this file according to the lab instructions.
//

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <specstrings.h>

struct Context {
	int width;
	int height;
	float aspect;
	GLFWwindow *window;
	GLuint program;
	GLuint program_cube;
	GLuint defaultVAO;
	float elapsed_time;
	float zoom_factor;
};
int main(void)
{
	Context ctx;

	// Create a GLFW window
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	ctx.width = 900;
	ctx.height = 900;
	ctx.aspect = float(ctx.width) / float(ctx.height);
	ctx.window = glfwCreateWindow(ctx.width, ctx.height, "Model viewer", nullptr, nullptr);
	ctx.zoom_factor = 1.0f;

	glfwMakeContextCurrent(ctx.window);
	glfwSetWindowUserPointer(ctx.window, &ctx);

	// Load OpenGL functions
	glewExperimental = true;
	GLenum status = glewInit();
	if (status != GLEW_OK) {
		std::cerr << "Error: " << glewGetErrorString(status) << std::endl;
		std::exit(EXIT_FAILURE);
	}
	std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

	// Initialize GUI
	//v ImGui_ImplGlfwGL3_Init(ctx.window, false /*do not install callbacks*/);

	// Initialize rendering
	glGenVertexArrays(1, &ctx.defaultVAO);
	glBindVertexArray(ctx.defaultVAO);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	// Start rendering loop
	while (!glfwWindowShouldClose(ctx.window)) {
		glfwPollEvents();
		ctx.elapsed_time = glfwGetTime();
		//display(ctx);
		glfwSwapBuffers(ctx.window);
	}

	// Shutdown
	glfwDestroyWindow(ctx.window);
	glfwTerminate();
	std::exit(EXIT_SUCCESS);
}

