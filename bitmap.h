
#include "head.h"

typedef unsigned char uchar;
typedef unsigned int uint;

uchar* LoadBmp(char* fileName,BITMAPINFOHEADER * bitmap)
{
	FILE *ptrFile=fopen(fileName,"rb");
	if(ptrFile==NULL)
		return NULL;
	BITMAPFILEHEADER header;
	fread(&header,sizeof(BITMAPFILEHEADER),1,ptrFile);
	
	if(header.bfType!=BITMAP_ID)
	{
		fclose(ptrFile);
		return NULL;
	}

	fread(bitmap,sizeof(BITMAPINFOHEADER),1,ptrFile);
	fseek(ptrFile,header.bfOffBits,SEEK_SET);	
	uchar* image=(uchar*)malloc(bitmap->biSizeImage);
	fread(image,1,bitmap->biSizeImage,ptrFile);
	
	if(!image)
	{
		fclose(ptrFile);
		return NULL;
	}

	int index=0;
	uchar tmp;
	for(;index<bitmap->biSizeImage;index+=3)
	{
		tmp=image[index+2];
		image[index+2]=image[index];
		image[index]=tmp;
	}

	fclose(ptrFile);
	return image;
}
bool LoadTexture(char *filename, GLuint &texture)
{	AUX_RGBImageRec *pImage = NULL;
	pImage = auxDIBImageLoad(filename);
	if(pImage == NULL)		return false;
	glGenTextures(1, &texture);	
	glBindTexture    (GL_TEXTURE_2D,texture);
	gluBuild2DMipmaps(GL_TEXTURE_2D,4, pImage->sizeX, 
					  pImage->sizeY,GL_RGB, GL_UNSIGNED_BYTE,pImage->data);
	free(pImage->data);
	free(pImage);	
	return true;
}

uchar* LoadBitmapWithAlpha(char* fileName,BITMAPINFOHEADER *bitmap)
{
	uchar* tmpImage=LoadBmp(fileName,bitmap);
	uchar* imageWithAlpha=(uchar*)malloc(bitmap->biSizeImage*4/3);
	if(tmpImage==NULL||imageWithAlpha==NULL)
		return NULL;
	uint des,src;
	for(des=0,src=0;src<bitmap->biSizeImage;des+=4,src+=3)
	{
		if(tmpImage[src]==0&&tmpImage[src+1]==0&&tmpImage[src+2]==0)
			imageWithAlpha[des+3]=0;
		else
			imageWithAlpha[des+3]=255;
		imageWithAlpha[des]=tmpImage[src];
		imageWithAlpha[des+1]=tmpImage[src+1];
		imageWithAlpha[des+2]=tmpImage[src+2];
	}
	free(tmpImage);
	return imageWithAlpha;
}


void LoadTextureWithAlpha(char* fileName,GLuint& tex)
{
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_2D,tex);
	BITMAPINFOHEADER bitmap;
	uchar* pImage=LoadBitmapWithAlpha(fileName,&bitmap);
	gluBuild2DMipmaps(GL_TEXTURE_2D,4,bitmap.biWidth,bitmap.biHeight,GL_RGBA,GL_UNSIGNED_BYTE,pImage);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
	
	free(pImage);

}


