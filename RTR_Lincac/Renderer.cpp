#include"Renderer.h"

Renderer::Renderer() {
	object = std::make_shared<Sphere>();
	framebuffer = std::make_shared<FrameBuffer>();
}

Renderer::~Renderer() {

}

void Renderer::render() {
	if (ChangeObj) {
		reset();
		if (dir == "Cube") {
			object = std::make_shared<Cube>();
			_PlaneY = -1.0f;
			center = glm::vec3(0.0);
			camera.update(center);
			projection = glm::perspective(glm::radians(camera.Zoom), GLfloat(HEIGHT) / GLfloat(HEIGHT), camera.nearPlane, camera.farPlane);
			view = camera.get_viewMatrix();
		}
		else if (dir == "Sphere") {
			object = std::make_shared<Sphere>();
			_PlaneY = -1.0f;
			center = glm::vec3(0.0);
			camera.update(center);
			projection = glm::perspective(glm::radians(camera.Zoom), GLfloat(HEIGHT) / GLfloat(HEIGHT), camera.nearPlane, camera.farPlane);
			view = camera.get_viewMatrix();
		}
		else {
			object = std::make_shared<Model>(dir.c_str());  
		}

		std::cout << "load successed" << std::endl;
		framebuffer->update();
		ChangeObj = false;
		dir = "";
	}

	framebuffer->render(object);
}

GLuint Renderer::get_image() {
	return framebuffer->get_textureID();
}

std::shared_ptr<Object>& Renderer::get_object() {
	return object;
}

void Renderer::set_object(std::shared_ptr<Object>& obj) {
	object = obj;
}

void Renderer::reset() {
	Albedo = 0;
	Normal = 0;
	Specular = 0;
	Roughness = 0;
	Metallic = 0;
	Ao = 0;

	use_albedoMap = false;
	use_normalMap = false;
	use_specularMap = false;
	use_roughnessMap = false;
	use_metallicMap = false;
	use_aoMap = false;

	vertexNum = 0;
	triangleNum = 0;
}