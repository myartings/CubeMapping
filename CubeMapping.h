#pragma once
#ifndef _CUBEMAPPING_
#define _CUBEMAPPING_
#include "stdafx.h"
#include "head.h"

static GLenum cubeFaces[6] = {
 GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT,
 GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT,
 GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT,
 GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT,
 GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT,
 GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT
};


class CubeMapping
{

public:
	CubeMapping(void);
public:
	virtual ~CubeMapping(void);

public:
	void Init();
	void DoCubeMapping();
	void DoDraw();
	void test();
private:
	void DrawCube();
	void DrawTorus();
	void DrawTeaport();
	void DrawSphere();
	void DoInput();
	float xrot,yrot;
	float xspeed,yspeed;
	float z;
	int filter;

	GLuint cubeTexture;
};

#endif
