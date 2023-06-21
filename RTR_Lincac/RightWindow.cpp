#include"RightWindow.h"

RightWindow::RightWindow() {
	Rwidth =  WIDTH - HEIGHT;
	Rheight = HEIGHT;
}

RightWindow::~RightWindow() {}

void RightWindow::render(const GLuint& image) {
	ImGui::SetNextWindowPos(ImVec2(GLfloat(Rheight), 0), ImGuiCond_None);
	ImGui::SetNextWindowSize(ImVec2(GLfloat(Rwidth), GLfloat(Rheight) / 2.0f), ImGuiCond_None);
	{
		ImGui::Begin("Operate Object", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);

		if (Mode == "BlinPhone") {
			ImGui::SliderFloat("Gloss", &_Gloss, 8.0f, 128.0f);
			ImGui::ColorEdit3("_Specular", (float*)&_Specular);
			ImGui::ColorEdit3("_Color", (float*)&_Color);
		}
		else if (Mode == "PBR") {
			ImGui::ColorEdit3("_Color", (float*)&_Color);
			ImGui::SliderFloat("_Roughness", &_Roughness, 0.0f,1.0f);
			ImGui::SliderFloat("_Metallic", &_Metallic, 0.0f, 1.0f);
			ImGui::SliderFloat("_Ao", &_Ao, 0.0f, 1.0f);

			ImGui::Image(use_albedoMap ? (void*)(intptr_t)Roughness : nullptr, ImVec2(100.0f, 100.0f));
			if (ImGui::Button("change metallic")) {
				dir = OpenFileDialog();
				if (dir != "") {
					Roughness = LoadTexture(dir.c_str());
					std::cout << "load texture successed" << std::endl;
					use_roughnessMap = true;
				}
			}
			ImGui::Image(use_normalMap ? (void*)(intptr_t)Metallic : nullptr, ImVec2(100.0f, 100.0f));
			if (ImGui::Button("change roughness")) {
				dir = OpenFileDialog();
				if (dir != "") {
					Metallic = LoadTexture(dir.c_str());
					std::cout << "load texture successed" << std::endl;
					use_metallicMap = true;
				}
			}
			ImGui::Image(use_albedoMap ? (void*)(intptr_t)Ao : nullptr, ImVec2(100.0f, 100.0f));
			if (ImGui::Button("change ao")) {
				dir = OpenFileDialog();
				if (dir != "") {
					Ao = LoadTexture(dir.c_str());
					std::cout << "load texture successed" << std::endl;
					use_aoMap = true;
				}
			}
		}

		ImGui::Image(use_albedoMap ? (void*)(intptr_t)Albedo : nullptr, ImVec2(100.0f, 100.0f));
		if (ImGui::Button("change albedo")) {
			dir = OpenFileDialog();
			if (dir != "") {
				Albedo = LoadTexture(dir.c_str());
				std::cout << "load texture successed" << std::endl;
				use_albedoMap = true;
			}
		}
		ImGui::Image(use_normalMap ? (void*)(intptr_t)Normal : nullptr, ImVec2(100.0f, 100.0f));
		if (ImGui::Button("change normal")) {
			dir = OpenFileDialog();
			if (dir != "") {
				Normal = LoadTexture(dir.c_str());
				std::cout << "load texture successed" << std::endl;
				use_normalMap = true;
			}
		}

		ImGui::End();
	}

	ImGui::SetNextWindowPos(ImVec2(GLfloat(Rheight), GLfloat(Rheight) / 2.0f), ImGuiCond_None);
	ImGui::SetNextWindowSize(ImVec2(GLfloat(Rwidth), GLfloat(Rheight) / 2.0f), ImGuiCond_None);
	{
		ImGui::Begin("Operate World", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);

		ImGui::ColorEdit3("LightCol", (float*)&LightCol);
		ImGui::Checkbox("use_skybox", &use_skybox);
		ImGui::InputFloat("Near Plane", &_near);
		ImGui::InputFloat("Far Plane", &_far);
		ImGui::SliderFloat("Light Vol", &LightVol,1.0f,20.0f);

		ImGui::End();
	}
}