#pragma once

#ifndef _WINDOW_H
#define _WINDOW_H

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include<imgui/imgui.h>
#include<imgui/imgui_impl_glfw.h>
#include<imgui/imgui_impl_opengl3.h>

#include<list>

#include<iostream>
#include"loadresource.h"

class Window {
public:
	virtual void render(const GLuint& image = -1) = 0;
};

#endif // !_WINDOW_H
