#pragma once

#ifndef _IRRADIANCE_H
#define _IRRADIANCE_H

#include"Help.h"

class Irradiance {
public:
	Irradiance(GLuint texture);
	~Irradiance();
	void update(GLuint texture);
private:
	unsigned int textureID;
	unsigned int captureFBO;
	unsigned int captureRBO;
	std::shared_ptr<Shader> shader;
};

#endif // !_IRRADIANCE_H
