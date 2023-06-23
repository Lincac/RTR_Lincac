#include"Program.h"

#include<imgui/imgui.h>
#include<imgui/imgui_impl_glfw.h>
#include<imgui/imgui_impl_opengl3.h>

void processInput(GLFWwindow* window);
void mouse_call_back(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

Program::Program() {
	init();

	Lwindow = std::make_shared<LeftWindow>();
	Rwindow = std::make_shared<RightWindow>();
	renderer = std::make_shared<Renderer>();
	
	// 加载天空盒资源
	GLuint skybox = LoadSkyBox("image/hdr/newport_loft.hdr");
	textures.emplace("skyboxMap", skybox);

	// 加载PBR资源
	lut = std::make_shared<LUT>();
	prefilter = std::make_shared<Prefilter>(skybox);
	irradiance = std::make_shared<Irradiance>(skybox);

	std::shared_ptr<Shader> pbr = std::make_shared<Shader>("shader/pbr/pbr.vs", "shader/pbr/pbr.fs");
	pbr->use();
	pbr->setInt("Albedo", 0);
	pbr->setInt("Normal", 1);
	pbr->setInt("Specular", 2);
	pbr->setInt("Metallic", 3);
	pbr->setInt("Roughness", 4);
	pbr->setInt("Ao", 5);
	pbr->setInt("irradianceMap", 6);
	pbr->setInt("prefilterMap", 7);
	pbr->setInt("LUTMap", 8);
	hashShader.emplace("PBR", pbr);
	shader = pbr;

	std::shared_ptr<Shader> blinphone = std::make_shared<Shader>("shader/blinphone/blinphone.vs", "shader/blinphone/blinphone.fs");
	blinphone->use();
	blinphone->setInt("Albedo", 0);
	blinphone->setInt("Normal", 1);
	hashShader.emplace("BlinPhone", blinphone);

	std::shared_ptr<Shader> dataView = std::make_shared<Shader>("shader/dataView/view.vs", "shader/dataView/view.fs");
	dataView->use();
	dataView->setInt("show_texture", 0);
	hashShader.emplace("dataView", dataView);
}

Program::~Program(){}

void Program::run() {
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = float(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glPolygonMode(GL_FRONT_AND_BACK, renderMode);

		processInput(window);

		projection = glm::perspective(glm::radians(camera.Zoom), GLfloat(HEIGHT) / GLfloat(HEIGHT), camera.nearPlane, camera.farPlane);
		view = camera.get_viewMatrix();
		camera.farPlane = _far;
		camera.nearPlane = _near;

		renderer->render();

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		Lwindow->render(renderer->get_image());
		Rwindow->render();

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwTerminate();
}

void Program::init() {
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);

	window = glfwCreateWindow(WIDTH, HEIGHT	, "RTR_Lincac", nullptr, nullptr);
	if (window == nullptr) {
		std::cout << "error to create window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSetCursorPosCallback(window, mouse_call_back);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);
	glfwSetScrollCallback(window, scroll_callback);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "error to load glad" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 450");
}


void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

void mouse_call_back(GLFWwindow* window, double xpos, double ypos) {

	currentx = WIDTH - static_cast<float>(xpos);
	currenty = HEIGHT - static_cast<float>(ypos);

	if (mouseClick)
	{
		float deltax = currentx - lastx;
		float deltay = currenty - lasty;

		camera.ProcessMouseButton(deltax, deltay);
	}
	lastx = currentx;
	lasty = currenty;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	if (currentx >= (WIDTH - HEIGHT))
		camera.ProcessMouseScroll(float(yoffset));
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
	glViewport(0, 0, width, height);
	WIDTH = width;
	HEIGHT = height;
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS && currentx >= (WIDTH - HEIGHT))
		mouseClick = true;
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE && currentx >= (WIDTH - HEIGHT))
		mouseClick = false;
}
