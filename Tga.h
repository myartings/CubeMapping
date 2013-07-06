#ifndef _TGA_H_
#define _TGA_H_
#include "stdafx.h"
#include "head.h"

struct	TextureImage								// 创建加载TGA图像文件结构
{
	GLubyte	*imageData;								// 图像数据指针
	GLuint	bpp;									// 每个数据所占的位数（必须为24或32）
	GLuint	width;									// 图像宽度
	GLuint	height;									// 图像高度
	GLuint	texID;									// 纹理的ID值
};

struct TGAHeader
{
	GLubyte header[12];
};


struct TGA
{
	GLubyte header[6];				// 控制前6个字节
	GLuint bytesPerPixel;			// 每像素的字节数 (3 或 4)
	GLuint imageSize;				// 控制存储图像所需的内存空间
	GLuint type;				// 图像类型 GL_RGB 或 GL_RGBA
	GLuint Height;				// 图像的高度
	GLuint Width;				// 图像宽度
	GLuint Bpp;				// 每像素的比特数 (24 或 32)
};

bool LoadTGA(TextureImage *texture, char *fileName);
bool LoadUncompassTGA(TextureImage *texture, FILE* pFile);
bool LoadCompassTGA(TextureImage *texture, FILE* pFile);

#endif