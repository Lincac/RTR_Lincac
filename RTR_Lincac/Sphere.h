#pragma once

#ifndef _SPHERE_H
#define _SPHERE_H

#include"Object.h"

class Sphere : public Object {
public:
	Sphere();
	~Sphere();

	virtual void render() override;
private:
	GLuint VAO;
	int indexCount;
	void init();
};

#endif // !_SPHERE_H
