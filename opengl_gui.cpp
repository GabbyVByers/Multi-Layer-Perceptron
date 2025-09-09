
#include "opengl.h"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"

void OpenGL::initImGui() const
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImPlot::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.FontGlobalScale = 2.0f;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void OpenGL::renderGUI()
{
	static std::vector<float> costHistory;
	costHistory.push_back(perceptron->currentCost);

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Debugger");

	ImGui::Text("Rendering Time (ms): %g", renderingTime);
	ImGui::Text("Training Time  (ms): %g", perceptron->trainingTime);
	ImGui::Text("Network Cost : %g", perceptron->currentCost);
	ImGui::Text("Expected Value: %i", perceptron->getCurrExpectedValue());

	if (ImGui::Button("Enable VSYNC"))
		enableVSYNC();
	ImGui::SameLine();
	if (ImGui::Button("Disable VSYNC"))
		disableVSYNC();

	if (ImGui::Button("Toggle Training"))
		perceptron->doContinuousTraining = !perceptron->doContinuousTraining;

	if (ImGui::Button("Single Step"))
		perceptron->putThatClankerToWork();

	if (ImPlot::BeginPlot("Cost Function", ImVec2(1, 1)))
	{
		ImPlot::PlotLine("Cost", &costHistory[0], costHistory.size(), 1.0f / costHistory.size());
		ImPlot::EndPlot();
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

