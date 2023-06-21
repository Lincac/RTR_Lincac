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

Program::~Program(){}

void Program::run() {
	while (!glfwWindowShouldClose(window))
	{
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
