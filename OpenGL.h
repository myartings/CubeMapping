#ifndef _OPENGL_
#define _OPENGL_
#include "stdafx.h"
#include "head.h"


class OpenGL
{
public:
	OpenGL();
	~OpenGL();
public:
	bool Init();
	void Reshape(int width,int height);
	void Display();

private:
	void InitLight();
};


#endif