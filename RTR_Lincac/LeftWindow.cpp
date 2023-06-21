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
					shader = std::make_shared<Shader>("shader/blinphone/blinphone.vs", "shader/blinphone/blinphone.fs");
					shader->use();
					shader->setInt("Albedo", 0);
					shader->setInt("Normal", 1);
				}
				if (ImGui::MenuItem("PBR")) {
					shader = std::make_shared<Shader>("shader/pbr/pbr.vs", "shader/pbr/pbr.fs");
					shader->use();
					shader->setInt("Albedo", 0);
					shader->setInt("Normal", 1);
					shader->setInt("Metallic", 2);
					shader->setInt("Roughness", 3);
					shader->setInt("Ao", 4);
					shader->setInt("irradianceMap", 5);
					shader->setInt("prefilterMap", 6);
					shader->setInt("LUTMap", 7);
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

