#include "stdafx.h"
#include "Tga.h"

#include "CubeMapping.h"

TextureImage* cubeTextures[6];
CubeMapping::CubeMapping(void)
{
	this->Init();
	this->DoCubeMapping();
}

CubeMapping::~CubeMapping(void)
{
	for(int i=0;i<6;i++)
	{
		if(cubeTextures[i])
		{
			if(cubeTextures[i]->imageData)
				free(cubeTextures[i]->imageData);
			free(cubeTextures[i]);
		}
	}

	glDeleteTextures(1,&cubeTexture);
}

void CubeMapping::Init()
{
	xspeed=yspeed=0;
	xrot=yrot=0;
	z=-6;
	filter=1;

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	for(int i=0;i<6;i++)
	{
		cubeTextures[i]=(TextureImage*)malloc(sizeof(TextureImage));
	}
}

void CubeMapping::DoCubeMapping()
{
	char *imgFile[6] = { "image/pos_x.tga", "image/neg_x.tga", "image/pos_y.tga", "image/neg_y.tga", "image/pos_z.tga", "image/neg_z.tga" };
	
	glGenTextures(1, &cubeTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, cubeTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for(int i=0;i<6;i++)
	{	
		LoadTGA(cubeTextures[i],imgFile[i]);
		//gluBuild2DMipmaps(cubeFaces[i],GL_RGB,cubeTextures[i]->width,cubeTextures[i]->height,GL_RGB,GL_UNSIGNED_BYTE,cubeTextures[i]->imageData);
		glTexImage2D(cubeFaces[i], 0, GL_RGB, cubeTextures[i]->width, cubeTextures[i]->height, 0, GL_RGB, GL_UNSIGNED_BYTE, cubeTextures[i]->imageData);
		free(cubeTextures[i]->imageData);
		cubeTextures[i]->imageData = NULL;
	}
		
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	
	glEnable(GL_TEXTURE_CUBE_MAP_EXT);
	glEnable(GL_NORMALIZE);	
}

void CubeMapping::DoDraw()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP_EXT,cubeTexture);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	DrawCube();

	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_REFLECTION_MAP_EXT);

	//glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT);//Diffuse Lighting
	//glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT);
	//glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP_EXT);

	//glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_TEXTURE_GEN_MODE);//Diffuse Lighting
	//glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_TEXTURE_GEN_MODE);
	//glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_TEXTURE_GEN_MODE);

	DoInput();
	switch(filter)
	{
	case 1:
		DrawSphere();
		break;
	case 2:
		DrawTeaport();
		break;
	case 3:
		DrawTorus();
		break;
	default:
		break;
	}
	xrot+=xspeed;
	yrot+=yspeed;
}

void CubeMapping::DrawCube()
{
	glPushMatrix();
	glutSolidCube(20.0f);
	glPopMatrix();
}

void CubeMapping::DrawTorus()
{
	glPushMatrix();
	glTranslatef(0,0,z);
	glRotatef(xrot,1,0,0);
	glRotatef(yrot,0,1,0);

	glutSolidTorus(0.2,0.5,32,32);
	glPopMatrix();
}
void CubeMapping::DrawTeaport()
{
	glPushMatrix();
	glTranslatef(0,0,z);
	glRotatef(xrot,1,0,0);
	glRotatef(yrot,0,1,0);

	glutSolidTeapot(1.0f);
	glPopMatrix();
}

void CubeMapping::DrawSphere()
{
	glPushMatrix();
	glTranslatef(0,0,z);
	glRotatef(xrot,1,0,0);
	glRotatef(yrot,0,1,0);
	
	glutSolidSphere(1.0,50,50);
	glPopMatrix();
}

void CubeMapping::DoInput()
{
	if(KEY_DOWN(VK_UP))
		xspeed-=0.02;
	if(KEY_DOWN(VK_DOWN))
		xspeed+=0.02;
	if(KEY_DOWN(VK_LEFT))
		yspeed-=0.02;
	if(KEY_DOWN(VK_RIGHT))
		yspeed+=0.02;
	if(KEY_DOWN('1'))
		filter=1;
	if(KEY_DOWN('2'))
		filter=2;
	if (KEY_DOWN('3'))
		filter=3;
	if(KEY_DOWN('W'))
		z-=0.02;
	if(KEY_DOWN('S'))
		z+=0.02;

}

void CubeMapping::test()
{
	TextureImage* tt=(TextureImage*)malloc(sizeof(TextureImage));
	if(!tt)
	{
		MessageBox(NULL,_T("无法分配内存"),_T("错误"),MB_OK);
		return ;
	}
	LoadTGA(tt,"image/cm_left.tga");
	glEnable(GL_TEXTURE_2D);
	GLuint tex;
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_2D,tex);

	gluBuild2DMipmaps(GL_TEXTURE_2D,GL_RGB,tt->width,tt->height,GL_RGB,GL_UNSIGNED_BYTE,tt->imageData);
	//glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,tt->width,tt->height,0,GL_RGB,GL_UNSIGNED_BYTE,tt->imageData);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);


	glBegin(GL_QUADS);
	{
		glTexCoord2f(0,0);glVertex3f(-1,-1,-2);
		glTexCoord2f(1,0);glVertex3f(1,-1,-2);
		glTexCoord2f(1,1);glVertex3f(1,1,-2);
		glTexCoord2f(0,1);glVertex3f(-1,1,-2);		
	}
	glEnd();
}








