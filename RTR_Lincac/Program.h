#pragma once

#ifndef _PROGRAM_H
#define _PROGRAM_H

#include"Renderer.h"
#include"LeftWindow.h"
#include"RightWindow.h"
#include"LUT.h"
#include"Prefilter.h"
#include"Irradiance.h"
#include"loadresource.h"

class Program {
public:
	Program();
	~Program();

	void run();
private:
	void init();

	GLFWwindow* window;
	std::shared_ptr<LeftWindow> Lwindow;
	std::shared_ptr<RightWindow> Rwindow;
	std::shared_ptr<Renderer> renderer;
	
	// 资源对象
	std::shared_ptr<LUT> lut;
	std::shared_ptr<Prefilter> prefilter;
	std::shared_ptr<Irradiance> irradiance;
};

#endif // !_PROGRAM_H
