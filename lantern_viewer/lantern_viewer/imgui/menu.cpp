#include "menu.h"

#include "lib/imgui.h"
#include "lib/imgui_impl_glfw_gl3.h"

static float max_fps = 0.0;
static float min_fps = 100000.0;


void initImGui(Context ctx)
{
	ImGui_ImplGlfwGL3_Init(ctx.window, false /*do not install callbacks*/);
}

void displayImGui(Context &ctx)
{

	ImGui_ImplGlfwGL3_NewFrame();
	ImGui::Begin("Settings");

	if (ImGui::CollapsingHeader("Global Settings"))
	{
		ImGui::Checkbox("Enable lantern (disable will show sphere)", (bool*)(&ctx.lantern_on));
	}
	if (ImGui::CollapsingHeader("Material Settings"))
	{
		ImGui::Checkbox("Use L0", (bool*)(&ctx.material_settings.use_L0));
		ImGui::Checkbox("Use ambient IBL", (bool*)(&ctx.material_settings.use_ambient_IBL));

		ImGui::Checkbox("Use albedo map", (bool*) (&ctx.material_settings.use_albedo_map));
		ImGui::ColorEdit3("Ambient Color", &ctx.material_settings.albedo_color[0]);

		ImGui::Checkbox("Use roughness map", (bool*)(&ctx.material_settings.use_roughness_map));
		ImGui::DragFloat("Roughness value", &ctx.material_settings.roughness_value, 0.01f, 0.0f, 1.0f);

		ImGui::DragFloat("Light X", &ctx.lights.position.x, 1.0f);
		ImGui::DragFloat("Light Y", &ctx.lights.position.y, 1.0f);
		ImGui::DragFloat("Light Z", &ctx.lights.position.z, 1.0f);
		ImGui::ColorEdit3("Light Color", &ctx.lights.color[0]);
	}

	if (ImGui::CollapsingHeader("Statistics")) {
		// From https://github.com/ocornut/imgui/issues/1200
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		min_fps = glm::min(ImGui::GetIO().Framerate, min_fps);
		ImGui::Text("Min FPS %.1f FPS", min_fps);
		max_fps = glm::max(ImGui::GetIO().Framerate, max_fps);
		ImGui::Text("Max FPS %.1f FPS", max_fps);

	}
	ImGui::End();
	ImGui::Render();
}
