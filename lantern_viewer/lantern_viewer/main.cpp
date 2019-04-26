#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw_gl3.h"

#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <specstrings.h>
#include <string>

#include <Windows.h>
#include <Pathcch.h>
#include <filesystem>
#include <assimp/Importer.hpp>      // C++ importer interface
#include "utils.h"

#include <boost/algorithm/string/replace.hpp>

struct Context
{
	int width;
	int height;
	float aspect;

	GLFWwindow* window;
	GLuint program;
	GLuint program_cube;

	GLuint triangleVAO;
	GLuint positionVBO;
	GLuint colorVBO;
	GLuint defaultVAO;

	float elapsed_time;
	float zoom_factor;
};

std::string getExecPath()
{
	char result[MAX_PATH];
	std::string path = std::string(result, GetModuleFileNameA(nullptr, result, MAX_PATH));
	path = path.substr(0, path.find_last_of("\\/"));
	return boost::replace_all_copy(path, "\\", "/");
}

void drawTriangle(GLuint program, GLuint vao)
{
	glUseProgram(program);

	glBindVertexArray(vao);
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glUseProgram(0);
}


GLuint createTriangleVAO()
{
	const GLfloat vertices[] = {
		0.0f, 0.5f, 0.0f,
		-0.5f,-0.5f, 0.0f,
		0.5f,-0.5f, 0.0f,
	};

	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);

	return vao;
}

void display(Context &ctx)
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	drawTriangle(ctx.program, ctx.triangleVAO);
}


void init(Context& ctx)
{
	std::cout << getExecPath() + "/shaders/triangle.vert" << std::endl;
	ctx.program = loadShaderProgram(getExecPath() + "/shaders/mesh.vert",
		getExecPath() + "/shaders/mesh.frag");

	ctx.triangleVAO = createTriangleVAO();
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

	ctx.width = 1100;
	ctx.height = 900;
	ctx.aspect = float(ctx.width) / float(ctx.height);
	ctx.window = glfwCreateWindow(ctx.width, ctx.height, "Lantern Viewer", nullptr, nullptr);
	ctx.zoom_factor = 1.0f;

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

	glm::vec3 m = glm::vec3(1.0f);
	bool hej[] = {true};
	// Start rendering loop
	while (!glfwWindowShouldClose(ctx.window))
	{
		glfwPollEvents();
		//ctx.elapsed_time = glfwGetTime();
		display(ctx);
		/*glClearColor(0.5, 0.5, 0.5, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		ImGui_ImplGlfwGL3_NewFrame();
		ImGui::Begin("This is a title", hej);
		ImGui::Text("Magnus is cool");
		ImGui::Checkbox("Check me", hej);
		ImGui::End();
		ImGui::Render();
		Assimp::Importer importer;*/
		glfwSwapBuffers(ctx.window);
	}

	// Shutdown
	glfwDestroyWindow(ctx.window);
	glfwTerminate();
	std::exit(EXIT_SUCCESS);
}
