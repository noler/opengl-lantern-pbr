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
#include <imgui.h>
#include "imgui/lib/imgui_impl_glfw_gl3.h"

#define TARGET_FPS 60.0

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

	drawMeshes(ctx);

	displayImGui(ctx);
}

// Must be called at least once before cameraUpdate()
void initCamera(Context& ctx)
{
	ctx.camera.camera_projection.zoomFactor = 1.0f;
	ctx.camera.camera_projection.zNear = 0.1f;
	ctx.camera.camera_projection.zFar = 500.0f;
}

void loadTextures(Context& ctx)
{
	ctx.lantern_obj.texture.albedo = load2DTexture(getExecPath() + "/models/lantern/textures/png/lantern_albedo.png");
	ctx.lantern_obj.texture.ambient_occlusion = load2DTexture(
		getExecPath() + "/models/lantern/textures/png/lantern_ambient_occlusion.png");
	ctx.lantern_obj.texture.metallic = load2DTexture(
		getExecPath() + "/models/lantern/textures/png/lantern_metallic.png");
	ctx.lantern_obj.texture.opacity = load2DTexture(getExecPath() + "/models/lantern/textures/png/lantern_opacity.png");
	ctx.lantern_obj.texture.roughness = load2DTexture(
		getExecPath() + "/models/lantern/textures/png/lantern_roughness.png");
	ctx.lantern_obj.texture.normal = load2DTexture(getExecPath() + "/models/lantern/textures/png/lantern_normal.png");
}

void loadCubemaps(Context& ctx)
{
	ctx.skybox_obj.skybox_cubemap = loadCubemap(getExecPath() + "/cubemaps/LarnacaCastle/");
	ctx.skybox_obj.skybox_cubemap_mipmap = loadCubemapMipmap(getExecPath() + "/cubemaps/LarnacaCastle/prefiltered/");
}

void initLight(Context& ctx)
{
	Light light_src_1;
	light_src_1.position = glm::vec3(10, 10, 10);

	light_src_1.color = glm::vec3(0.98, 1.0 ,0.58);
	ctx.lights = light_src_1;
}

void init(Context& ctx)
{
	initializeTrackball(ctx);
	createCube(ctx);

	ctx.shader_lantern_base = loadShaderProgram(getExecPath() + "/shaders/mesh_base.vert",
	                                            getExecPath() + "/shaders/mesh_base.frag");

	ctx.shader_lantern_glass = loadShaderProgram(getExecPath() + "/shaders/mesh_glass.vert",
	                                             getExecPath() + "/shaders/mesh_glass.frag");

	ctx.shader_skybox = loadShaderProgram(getExecPath() + "/shaders/skybox_cube.vert",
	                                      getExecPath() + "/shaders/skybox_cube.frag");

	loadTextures(ctx);
	loadCubemaps(ctx);

	ctx.camera.position = glm::vec3(0, 0.0, 100.0);

	initLight(ctx);
	initCamera(ctx);
	updateCamera(ctx);

	ctx.material_settings = MaterialSettings();

	ModelManager manager;
	manager.loadModel(getExecPath() + "/models/lantern/lantern_obj.obj");

	std::vector<Mesh> meshes = manager.getMesh();

	ctx.lantern_obj.mesh_lantern_base = meshes.at(0);
	createMeshVAO(ctx, ctx.lantern_obj.mesh_lantern_base, &ctx.lantern_obj.mesh_lantern_baseVAO);

	ctx.lantern_obj.mesh_lantern_glass = meshes.at(1);
	createMeshVAO(ctx, ctx.lantern_obj.mesh_lantern_glass, &ctx.lantern_obj.mesh_lantern_glassVAO);

	ModelManager manager2;
	manager2.loadModel(getExecPath() + "/models/icosphere_unwrapped.obj");
	meshes = manager2.getMesh();
	ctx.sphere_obj.mesh_sphere = meshes.at(0);
	createMeshVAO(ctx, ctx.sphere_obj.mesh_sphere, &ctx.sphere_obj.mesh_sphere_VAO);
	
}

void reloadShaders(Context* ctx)
{
	glDeleteProgram(ctx->shader_lantern_base);
	ctx->shader_lantern_base = loadShaderProgram(getExecPath() + "/shaders/mesh_base.vert",
	                                             getExecPath() + "/shaders/mesh_base.frag");

	glDeleteProgram(ctx->shader_lantern_glass);
	ctx->shader_lantern_glass = loadShaderProgram(getExecPath() + "/shaders/mesh_glass.vert",
	                                              getExecPath() + "/shaders/mesh_glass.frag");

	glDeleteProgram(ctx->shader_skybox);
	ctx->shader_skybox = loadShaderProgram(getExecPath() + "/shaders/skybox_cube.vert",
	                                       getExecPath() + "/shaders/skybox_cube.frag");
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Forward event to GUI
	ImGui_ImplGlfwGL3_KeyCallback(window, key, scancode, action, mods);
	if (ImGui::GetIO().WantCaptureKeyboard) { return; } // Skip other handling

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

	std::string title = "Lantern Viewer";

	ctx.aspect = float(ctx.global_settings.width) / float(ctx.global_settings.height);
	ctx.window = glfwCreateWindow(ctx.global_settings.width, ctx.global_settings.height, title.c_str(), nullptr,
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

	double lasttime = glfwGetTime();
	// Start rendering loop
	while (!glfwWindowShouldClose(ctx.window))
	{
		glfwPollEvents();
		ctx.elapsed_time = glfwGetTime();

		display(ctx);
		glfwSwapBuffers(ctx.window);
		glfwSetWindowTitle(
			ctx.window, (title + " (FPS: " + std::to_string((int)round(ImGui::GetIO().Framerate)) + ")").c_str());

		// FPS Limiter credit: https://github.com/glfw/glfw/issues/1308
		while (glfwGetTime() < lasttime + 1.0 / TARGET_FPS)
		{
			// TODO: Put the thread to sleep, yield, or simply do nothing
		}
		lasttime += 1.0 / TARGET_FPS;
	}

	// Shutdown
	glfwDestroyWindow(ctx.window);
	glfwTerminate();
	std::exit(EXIT_SUCCESS);
}
