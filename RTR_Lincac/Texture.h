#ifndef  _TEXTURE_H
#define _TEXTURE_H

#include <glad/glad.h>
#include<iostream>	
#include<unordered_map>
#include<stb_image.h>

extern GLuint GenerateTexture2D(GLuint width, GLuint height, GLenum texture_type = GL_RGBA, GLenum Wrap_type = GL_REPEAT, GLenum Fileter_type = GL_NEAREST);
extern unsigned int LoadTexture(const char* path);
extern unsigned int loadHDRTexture(const char* path);

#endif // ! TEXTURE_H
