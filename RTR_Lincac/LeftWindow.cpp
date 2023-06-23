#include"LeftWindow.h"

LeftWindow::LeftWindow(){
	Lwidth = HEIGHT;
	Lheight = HEIGHT;
	directory = std::list<std::string>();
}

LeftWindow::~LeftWindow(){}

void LeftWindow::render(const GLuint& image) {
	ImGui::SetNextWindowPos(ImVec2(0.0, 0.0), ImGuiCond_None);
	ImGui::SetNextWindowSize(ImVec2(GLfloat(Lwidth), GLfloat(Lheight)), ImGuiCond_None);
	{
		ImGui::Begin("Draw", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File")) {
				if (ImGui::MenuItem("Open File")) {
					dir = OpenFileDialog();
					if (dir != "") {
						directory.push_back(dir);
						if (directory.size() >= 6) {
							directory.pop_front();
						}
						ChangeObj = true;
					}
				}
				if (ImGui::BeginMenu("Open history")) {
					for (auto& temp : directory) {
						if (ImGui::MenuItem(temp.c_str())) {
							std::cout << "Opening...." << std::endl;
							dir = temp;
							ChangeObj = true;
						}
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Exit")) {
					glfwTerminate();
					exit(-1);
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("System Object"))
			{
				if (ImGui::MenuItem("Cube")) {
					ChangeObj = true;
					dir = "Cube";
				}
				if (ImGui::MenuItem("Sphere")) {
					ChangeObj = true;
					dir = "Sphere";
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("RenderMode"))
			{
				if (ImGui::MenuItem("BlinPhone")) {
					Mode = "BlinPhone";
					preMode = "BlinPhone";
					shader = hashShader.at("BlinPhone");
				}
				if (ImGui::MenuItem("PBR")) {
					Mode = "PBR";
					preMode = "PBR";
					shader = hashShader.at("PBR");
				}
				if (ImGui::MenuItem("Cartoon")) {

				}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::Image((void*)(intptr_t)image, ImGui::GetContentRegionAvail(), ImVec2(0, 1), ImVec2(1, 0));

		ImGui::End();
	}
}

