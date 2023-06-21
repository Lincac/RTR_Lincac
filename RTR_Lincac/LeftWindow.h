#pragma once

#ifndef _LEFTWINDOW_H
#define _LEFTWINDOW_H

#include"Window.h"

class LeftWindow : public Window{
public:
	LeftWindow();
	~LeftWindow();
	virtual void render(const GLuint& image = -1) override;
private:
	GLuint Lwidth,Lheight;
	std::list<std::string> directory;
};

#endif // !_LEFTWINDOW_H
