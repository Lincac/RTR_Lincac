#pragma once

#ifndef _PLANE_H
#define _PLANE_H

#include"Object.h"

class Plane {
public:
	Plane();
	~Plane();
	void render();
	void update();
private:
	GLuint VAO;
	std::shared_ptr<Shader> planeshader;
	void init();
};

#endif // !_PLANE_H
