#pragma once

#ifndef _PREFILTER_H
#define _PREFILTER_H

#include"Help.h"

class Prefilter {
public:
	Prefilter(GLuint texture);
	~Prefilter();
	void update(GLuint texture);
private:
	unsigned int textureID;
	unsigned int captureFBO;
	unsigned int captureRBO;
	std::shared_ptr<Shader> shader;
};


#endif // _PREFILTER_H
