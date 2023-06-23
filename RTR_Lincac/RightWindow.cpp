#include"RightWindow.h"

#include<imgui/imgui_internal.h>

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

		bool is = false;
		if (ImGui::BeginTabBar("bar", ImGuiTabBarFlags_None)) {

			if (ImGui::BeginTabItem("Light information")) {
				if (Mode != preMode) {
					renderMode = GL_FILL;
					Mode = preMode;
					shader = hashShader.at(preMode);
				}

				if (Mode == "BlinPhone") {
					ImGui::SliderFloat("Gloss", &_Gloss, 8.0f, 128.0f);
					ImGui::ColorEdit3("_Specular", (float*)&_Specular);
					ImGui::ColorPicker3("_Color", (float*)&_Color, ImGuiColorEditFlags_None | ImGuiColorEditFlags_DisplayRGB);
				}
				else if (Mode == "PBR") {
					ImGui::ColorPicker3("_Color", (float*)&_Color, ImGuiColorEditFlags_None | ImGuiColorEditFlags_DisplayRGB);
					ImGui::ColorEdit3("_Specular", (float*)&_Specular);
					ImGui::SliderFloat("_Roughness", &_Roughness, 0.0f, 1.0f);
					ImGui::SliderFloat("_Metallic", &_Metallic, 0.0f, 1.0f);
					ImGui::SliderFloat("_Ao", &_Ao, 0.0f, 1.0f);

					ImGui::Image(use_metallicMap ? (void*)(intptr_t)Metallic : nullptr, ImVec2(100.0f, 100.0f));
					if (ImGui::Button("change metallic")) {
						dir = OpenFileDialog();
						if (dir != "") {
							Metallic = LoadTexture(dir.c_str());
							std::cout << "load texture successed" << std::endl;
							use_metallicMap = true;
						}
					}

					ImGui::Image(use_roughnessMap ? (void*)(intptr_t)Roughness : nullptr, ImVec2(100.0f, 100.0f));
					if (ImGui::Button("change roughness")) {
						dir = OpenFileDialog();
						if (dir != "") {
							Roughness = LoadTexture(dir.c_str());
							std::cout << "load texture successed" << std::endl;
							use_roughnessMap = true;
						}
					}

					ImGui::Image(use_aoMap ? (void*)(intptr_t)Ao : nullptr, ImVec2(100.0f, 100.0f));
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
				ImGui::Image(use_specularMap ? (void*)(intptr_t)Specular : nullptr, ImVec2(100.0f, 100.0f));
				if (ImGui::Button("change specular")) {
					dir = OpenFileDialog();
					if (dir != "") {
						Specular = LoadTexture(dir.c_str());
						std::cout << "load texture successed" << std::endl;
						use_specularMap = true;
					}
				}

				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Model information")) {
				if (Mode != "dataView") {
					Mode = "dataView";
					shader = hashShader.at(Mode);
				}

				if (ImGui::TreeNode("Grid data")) {
					enum Mode
					{
						Vertex,
						triangle,
					};
					static int mode = 3;
					if (ImGui::RadioButton(std::string("Vertex                   " + std::to_string(vertexNum)).c_str(), mode == Vertex)) { mode = Vertex; renderMode = GL_POINT; }
					if (ImGui::RadioButton(std::string("triangle                 " + std::to_string(triangleNum)).c_str(), mode == triangle)) { mode = triangle; renderMode = GL_LINE;}
					if (ImGui::RadioButton("NULL", mode == 3)) { mode = 3; renderMode = GL_FILL; }
					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Texture Data")) {
					enum Texture
					{
						_Albedo,
						_Normal,
						_Roughness,
						_Metallic,
						_AO,
					};
					static int tex = 5;
					if (ImGui::RadioButton("Albedo                  " , tex == _Albedo)) {
						tex = _Albedo;
						show_texture = Albedo;
						show = true;
					}
					if (ImGui::RadioButton("Normal                 " , tex == _Normal)) {
						tex = _Normal;
						show_texture = Normal;
						show = true;
					}
					if (ImGui::RadioButton("Roughness           ", tex == _Roughness)) {
						tex = _Roughness;
						show_texture = Roughness;
						show = true;
					}
					if (ImGui::RadioButton("Metallic                ", tex == _Metallic)) {
						tex = _Metallic;
						show_texture = Metallic;
						show = true;
					}
					if (ImGui::RadioButton("AO							", tex == _AO)) {
						tex = _AO;
						show_texture = Ao;
						show = true;
					}
					if (ImGui::RadioButton("NULL", tex == 5)) {
						tex = 5;
						show_texture = 0;
						show = false;
					}

					ImGui::TreePop();
				}
				if (ImGui::TreeNode("Bone data")) {
					ImGui::TreePop();
				}

				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
	}

	ImGui::SetNextWindowPos(ImVec2(GLfloat(Rheight), GLfloat(Rheight) / 2.0f), ImGuiCond_None);
	ImGui::SetNextWindowSize(ImVec2(GLfloat(Rwidth), GLfloat(Rheight) / 2.0f), ImGuiCond_None);
	{
		ImGui::Begin("Operate World", NULL, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_MenuBar);

		if (ImGui::BeginTabBar("global bar", ImGuiTabBarFlags_None)) {
			if (ImGui::BeginTabItem("GLobal Setting"))
			{
				ImGui::ColorPicker3("LightCol", (float*)&LightCol, ImGuiColorEditFlags_PickerHueWheel |  ImGuiColorEditFlags_DisplayRGB);
				ImGui::SliderFloat("Light Vol", &LightVol, 1.0f, 20.0f);
				if (ImGui::DragFloat("Light Pos", &angle, 0.1f, 0.0f, 360.0f, "%.1f")) {
					LightPos.x = glm::cos(glm::radians(angle));
					LightPos.z = glm::sin(glm::radians(angle));
				}
				ImGui::Checkbox("use_skybox", &use_skybox);
				ImGui::InputFloat("Near Plane", &_near);
				ImGui::InputFloat("Far Plane", &_far);
				ImGui::EndTabItem();
			}

			if (ImGui::BeginTabItem("Animation"))
			{
				enum animation_Mode
				{
					rotate,
					swing,
					rotate_jump,
					hover,
					load_animation
				};
				static int mode = 6;
				if (ImGui::RadioButton("rotate", mode == rotate)) { 
					mode = rotate; 
					animation_mode = "rotate";
					display_animation = false;
					_time = 0;
				}
				if (ImGui::RadioButton("swing", mode == swing)) { 
					mode = swing; 
					animation_mode = "swing";
					display_animation = false;
					_time = 0;
				}
				if (ImGui::RadioButton("rotate_jump", mode == rotate_jump)) { 
					mode = rotate_jump; 
					animation_mode = "rotate_jump";
					display_animation = false;
					_time = 0;
				}
				if (ImGui::RadioButton("hover", mode == hover)) { 
					mode = hover; 
					animation_mode = "hover";
					display_animation = false;
					_time = 0;
				}
				if (ImGui::RadioButton("load_animation", mode == load_animation)) {
					mode = load_animation;
					animation_mode = "load_animation";
					animation_file = OpenFileDialog();
					if (animation_file != "") {
						_time = 1;
						display_animation = true;
					}
					else _time = 0;
				}
				if (ImGui::RadioButton("NULL", mode == 6)) { 
					mode = 6; 
					animation_mode = "NUL";
					display_animation = false;
					_time = 0;
				}
				ImGui::EndTabItem();
			}

			ImGui::EndTabBar();
		}

		ImGui::End();
	}
}