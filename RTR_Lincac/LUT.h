#pragma once

#ifndef _LUT_H
#define _LUT_H

#include"Help.h"

class LUT{
public:
	LUT();
	~LUT();
private:
	unsigned int textureID;
	unsigned int captureFBO;
	unsigned int captureRBO;
	std::shared_ptr<Shader> shader;
};

#endif // !_LUT_H
