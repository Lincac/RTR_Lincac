#pragma once

#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#include"Plane.h"
#include"Cube.h"
#include"Sphere.h"
#include"Animator.h"

class FrameBuffer {
public:
	FrameBuffer();
	~FrameBuffer();
	void render(std::shared_ptr<Object>& obj);
	GLuint get_textureID();
	void update();
private:
	GLuint FBO,FBOtextureID;
	std::shared_ptr<Plane> plane;
	std::shared_ptr<Shader> skybox;
	std::shared_ptr<Animator> animator;
	glm::mat4 get_animation(std::shared_ptr<Object>& obj);
};

#endif // !_FRAMEBUFFER_H
