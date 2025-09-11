
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
	static std::vector<float> costLongHistory;
	static int shortCounter = 0;
	static float costShortHistory = 0.0f;
	int maxShort = 100;

	if (perceptron->doContinuousTraining)
	{
		costShortHistory += perceptron->currentCost;
		shortCounter++;
		if (shortCounter >= maxShort)
		{
			float average = costShortHistory / (float)maxShort;
			costLongHistory.push_back(average);
			shortCounter = 0;
			costShortHistory = 0.0f;
		}
	}
	
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::Begin("Debugger");

	ImGui::Text("Rendering Time (ms): %g", renderingTime);
	ImGui::Text("Training Time  (ms): %g", perceptron->trainingTime);
	ImGui::Text("Network Cost : %g", perceptron->currentCost);
	ImGui::Text("Ground Truth: %i", perceptron->getCurrExpectedValue());
	ImGui::Text("Network's Choice: %i", perceptron->networkChoice);

	if (ImGui::Button("Enable VSYNC"))
		enableVSYNC();
	ImGui::SameLine();
	if (ImGui::Button("Disable VSYNC"))
		disableVSYNC();

	if (ImGui::Button("Toggle Training"))
		perceptron->doContinuousTraining = !perceptron->doContinuousTraining;

	if (ImGui::Button("Single Step"))
		perceptron->putThatClankerToWork();

	if (ImPlot::BeginPlot("Cost Function History", ImVec2(-1, -1), ImPlotFlags_NoInputs))
	{
		ImPlot::PlotLine("", &costLongHistory[0], costLongHistory.size(), 1.0f / costLongHistory.size());
		ImPlot::EndPlot();
	}

	ImGui::End();
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

