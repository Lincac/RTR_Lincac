#pragma once

#ifndef _CUBE_H
#define _CUBE_H

#include"Object.h"

class Cube : public Object{
public:
	Cube();
	~Cube();

	virtual void render() override;
private:
	GLuint VAO;
	void init();
};

#endif // !_CUBE_H
