#pragma once

#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#include"Plane.h"
#include"Cube.h"
#include"Sphere.h"
#include"Model.h"

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
};

#endif // !_FRAMEBUFFER_H
