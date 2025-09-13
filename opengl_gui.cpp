
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
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	if (!perceptron->isBeingBenchmarked)
	{
		ImGui::Begin("Debugger");

		ImGui::Text("FPS: %g", 1000.0 / mainLoopTime);
		ImGui::Text("Parameter Count: %i", perceptron->numParameters);
		ImGui::Text("Rendering Time (ms): %g", renderingTime);
		ImGui::Text("Training Time  (ms): %g", perceptron->trainingTime);
		ImGui::Text("Network Cost : %g", perceptron->currentCost);
		ImGui::Text("Ground Truth: %i", perceptron->getCurrExpectedValue());
		ImGui::Text("Network's Choice: %i", perceptron->networkChoice);

		ImGui::Text("Training Examples Per Batch");
		ImGui::SliderInt("", &perceptron->trainingExamplesPerFrame, 1, 256);

		if (ImGui::Button("    Enable  VSYNC     "))
			enableVSYNC();

		if (ImGui::Button("    Disable VSYNC     "))
			disableVSYNC();

		if (ImGui::Button("   Toggle Training    "))
			perceptron->isContinuouslyTraining = !perceptron->isContinuouslyTraining;

		if (ImGui::Button("     Next Example     "))
			perceptron->trainOnASingleExample(1);

		if (ImGui::Button("   Previous Example   "))
			perceptron->trainOnASingleExample(-1);

		if (ImGui::Button("     Reset Network    "))
			perceptron->initWeightsWithRandomValues();

		if (ImGui::Button("     Run Benchmark    "))
		{
			perceptron->correctlyCategorizedTestExamples = 0;
			perceptron->indexCurrTestExample = -1;
			perceptron->isContinuouslyTraining = false;
			perceptron->isBeingBenchmarked = true;
		}

		if (ImPlot::BeginPlot("Cost Function", ImVec2(-1, 500), ImPlotFlags_NoInputs))
		{
			std::vector<float>& costHistory = perceptron->longRunCostHistory;

			//ImPlot::SetupAxes("Iteration Batch", "Cost");
			ImPlot::SetupAxisLimits(ImAxis_X1, 0.0, (double)costHistory.size(), ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, 1.0, ImGuiCond_Always);

			ImPlot::PlotLine("", &costHistory[0], costHistory.size());
			ImPlot::EndPlot();
		}

		if (ImPlot::BeginPlot("Model Accuracy", ImVec2(-1, 500), ImPlotFlags_NoInputs))
		{
			std::vector<float>& accuracyHistory = perceptron->accuracyHistory;

			//ImPlot::SetupAxes("Iteration Batch", "Cost");
			ImPlot::SetupAxisLimits(ImAxis_X1, 0.0, (double)accuracyHistory.size(), ImGuiCond_Always);
			ImPlot::SetupAxisLimits(ImAxis_Y1, 0.0, 1.0, ImGuiCond_Always);

			ImPlot::PlotLine("", &accuracyHistory[0], accuracyHistory.size());
			ImPlot::EndPlot();
		}

		ImGui::End();
	}
	else
	{
		ImGui::Begin("Benchmarking");
		ImGui::Text("The Network has never been Trained");
		ImGui::Text("on These 10,000 Digits...");

		ImGui::Text("Accuracy: %g", perceptron->benchmarkAccuracy);

		if (ImGui::Button("     Run Benchmark    "))
		{
			perceptron->correctlyCategorizedTestExamples = 0;
			perceptron->indexCurrTestExample = -1;
			perceptron->isContinuouslyTraining = false;
			perceptron->isBeingBenchmarked = true;
		}

		if (ImGui::Button("   Exit Benchmarking  "))
		{
			perceptron->isBeingBenchmarked = false;
			perceptron->trainOnASingleExample(0);
		}

		ImGui::End();
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

