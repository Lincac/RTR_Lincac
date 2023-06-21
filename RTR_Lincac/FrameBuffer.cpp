#include"FrameBuffer.h"

FrameBuffer::FrameBuffer() : FBO(0), FBOtextureID(0){
	// ≥ı ºªØ÷°ª∫≥Â £® π”√”≤º˛MSAA£©
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	glGenTextures(1, &FBOtextureID);
	glBindTexture(GL_TEXTURE_2D, FBOtextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, HEIGHT, HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
 	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, FBOtextureID, 0);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	plane = std::make_shared<Plane>();
	skybox = std::make_shared<Shader>("shader/skybox/sky.vs", "shader/skybox/sky.fs");
}

FrameBuffer::~FrameBuffer(){}

void FrameBuffer::render(std::shared_ptr<Object>& obj) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, HEIGHT, HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(bgColor.x, bgColor.y, bgColor.z, 1.0f);

    glm::mat4 model = glm::mat4(1);
    shader->use();
    shader->setMat4("model", model);
    shader->setMat4("VP", projection * view);
    shader->setMat3("normalMatrix", glm::transpose(glm::inverse(glm::mat3(model))));
    shader->setVec3("LightCol", LightCol * LightVol);
    shader->setVec3("LightDir", glm::normalize(LightPos));
    shader->setVec3("viewPos", camera.Position);
    shader->setVec3("_Color", _Color);
    shader->setBool("use_albedoMap", use_albedoMap);
    shader->setBool("use_normalMap", use_normalMap);
    if (Mode == "BlinPhone") {
        shader->setFloat("Gloss", _Gloss);
        shader->setVec3("Ambient", ambient);
        shader->setVec3("_Specular", _Specular);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Albedo);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Normal);
    }
    else if (Mode == "PBR") {
        shader->setBool("use_roughnessMap", use_roughnessMap);
        shader->setBool("use_metallicMap", use_metallicMap);
        shader->setBool("use_aoMap", use_aoMap);
        shader->setBool("use_skybox", use_skybox);
        shader->setFloat("_Roughness", _Roughness);
        shader->setFloat("_Metallic", _Metallic);
        shader->setFloat("_Ao", _Ao);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Albedo);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Normal);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, Metallic);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, Roughness);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, Ao);

        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textures.at("irradianceMap"));
        glActiveTexture(GL_TEXTURE6);
        glBindTexture(GL_TEXTURE_CUBE_MAP, textures.at("prefilterMap"));
        glActiveTexture(GL_TEXTURE7);
        glBindTexture(GL_TEXTURE_2D, textures.at("LUTMap"));
    }
    else if (Mode == "Cartoon") {

    }
	obj->render();
	plane->render();

	if (use_skybox) {
		skybox->use();
		skybox->setInt("cubemap", 0);
		skybox->setMat4("view", view);
		skybox->setMat4("projection", projection);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, textures.at("skyboxMap"));
		renderCube();
	}

	// ±£¥Êøπæ‚≥›ÕºœÒ
	glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO); // –¥»ÎµΩƒ¨»œ÷°ª∫≥Â
	glBlitFramebuffer(0, 0, HEIGHT, HEIGHT, 0, 0, HEIGHT, HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint FrameBuffer::get_textureID() {
	return FBOtextureID;
}

void FrameBuffer::update() {
	plane->update();
}