#pragma once

#ifndef _RENDERER_H
#define _RENDERER_H

#include<iostream>
#include"FrameBuffer.h"

class Renderer {
public:
	Renderer();
	~Renderer();
	void render();
	GLuint get_image();

	std::shared_ptr<Object>& get_object();
	void set_object(std::shared_ptr<Object>& obj); 
private:
	std::shared_ptr<FrameBuffer> framebuffer;
	std::shared_ptr<Object> object;
};

#endif // !_RENDERER_H
