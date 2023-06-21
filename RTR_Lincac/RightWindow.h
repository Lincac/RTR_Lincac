#pragma once

#ifndef _RIGHTWINDOW_H
#define _RIGHTWINDOW_H

#include"Window.h"

class RightWindow : public Window {
public:
	RightWindow();
	~RightWindow();
	virtual void render(const GLuint& image = -1) override;
private:
	GLuint Rwidth, Rheight;
};

#endif // !_RIGHTWINDOW_H
