#ifndef _TGA_H_
#define _TGA_H_
#include "stdafx.h"
#include "head.h"

struct	TextureImage								// ��������TGAͼ���ļ��ṹ
{
	GLubyte	*imageData;								// ͼ������ָ��
	GLuint	bpp;									// ÿ��������ռ��λ��������Ϊ24��32��
	GLuint	width;									// ͼ����
	GLuint	height;									// ͼ��߶�
	GLuint	texID;									// �����IDֵ
};

struct TGAHeader
{
	GLubyte header[12];
};


struct TGA
{
	GLubyte header[6];				// ����ǰ6���ֽ�
	GLuint bytesPerPixel;			// ÿ���ص��ֽ��� (3 �� 4)
	GLuint imageSize;				// ���ƴ洢ͼ��������ڴ�ռ�
	GLuint type;				// ͼ������ GL_RGB �� GL_RGBA
	GLuint Height;				// ͼ��ĸ߶�
	GLuint Width;				// ͼ����
	GLuint Bpp;				// ÿ���صı����� (24 �� 32)
};

bool LoadTGA(TextureImage *texture, char *fileName);
bool LoadUncompassTGA(TextureImage *texture, FILE* pFile);
bool LoadCompassTGA(TextureImage *texture, FILE* pFile);

#endif