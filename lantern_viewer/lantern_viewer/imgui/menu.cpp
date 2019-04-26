#include "menu.h"

#include "lib/imgui.h"
#include "lib/imgui_impl_glfw_gl3.h"

void initImGui(Context ctx)
{
	ImGui_ImplGlfwGL3_Init(ctx.window, false /*do not install callbacks*/);
}

void displayImGui()
{
	bool dummy[] = { true };
	ImGui_ImplGlfwGL3_NewFrame();
	ImGui::Begin("This is a title", dummy);
	ImGui::Text("Magnus is cool");
	ImGui::Checkbox("Check me", dummy);
	ImGui::End();
	ImGui::Render();
}
