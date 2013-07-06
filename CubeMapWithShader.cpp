#include "StdAfx.h"
#include "CubeMapWithShader.h"
#include "Tga.h"
#include "math3d.h"
#include <iostream>

const char *vertexFile = "Reflection.vp";
const char *fragementFile = "Reflection.fp";
const char *skyVertexFile = "SkyBox.vp";
const char *skyFragementFile = "SkyBox.fp"; 

TextureImage* cubeTexturesShader[6];

using namespace std;

CubeMapWithShader::CubeMapWithShader(void)
{
}

CubeMapWithShader::~CubeMapWithShader(void)
{
	for(int i=0;i<6;i++)
	{
		if(cubeTexturesShader[i])
		{
			if(cubeTexturesShader[i]->imageData)
				free(cubeTexturesShader[i]->imageData);
			free(cubeTexturesShader[i]);
		}
	}

	glDeleteTextures(1,&cubeTexture);
}

void CubeMapWithShader::SetupRC()
{
	GLchar *vertexData;
	GLchar *fragementData;

	GLchar *skyBoxVertexData;
	GLchar *skyBoxFragementData;
	
	//init cubemapping
	Init();
	DoCubeMapping();

	//init shader
	int size = ShaderSize(vertexFile);
	vertexData = ReadShaderSource(vertexFile,size);
	
	size = ShaderSize(fragementFile);
	fragementData = ReadShaderSource(fragementFile,size);
	
	InstallShaders(reflectionProgramObject,vertexData,fragementData);

	size = ShaderSize(skyVertexFile);
	skyBoxVertexData = ReadShaderSource(skyVertexFile,size);

	size = ShaderSize(skyFragementFile);
	skyBoxFragementData = ReadShaderSource(skyFragementFile,size);

	InstallShaders(cubeProgramObject,skyBoxVertexData,skyBoxFragementData);
}

void CubeMapWithShader::Init()
{
	xspeed=yspeed=0;
	xrot=yrot=0;
	z=-6;
	filter=1;

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	for(int i=0;i<6;i++)
	{
		cubeTexturesShader[i]=(TextureImage*)malloc(sizeof(TextureImage));
	}
}

void CubeMapWithShader::DoCubeMapping()
{
	char *imgFile[6] = { "image/pos_x.tga", "image/neg_x.tga", "image/pos_y.tga", "image/neg_y.tga", "image/pos_z.tga", "image/neg_z.tga" };

	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	glGenTextures(1, &cubeTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP_EXT, cubeTexture);

	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP_EXT, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);  
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for(int i=0;i<6;i++)
	{	
		LoadTGA(cubeTexturesShader[i],imgFile[i]);
		//gluBuild2DMipmaps(cubeFaces[i],GL_RGB,cubeTextures[i]->width,cubeTextures[i]->height,GL_RGB,GL_UNSIGNED_BYTE,cubeTextures[i]->imageData);
		glTexImage2D(cubeShaderFaces[i], 0, GL_RGB, cubeTexturesShader[i]->width, cubeTexturesShader[i]->height, 0,GL_RGB , GL_UNSIGNED_BYTE, cubeTexturesShader[i]->imageData);
		free(cubeTexturesShader[i]->imageData);
		cubeTexturesShader[i]->imageData = NULL;
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

int CubeMapWithShader::ShaderSize(const char *fileName )
{
	FILE *fp = fopen(fileName,"rt");
	int count;
	if (!fp)
		MessageBox(NULL,_T("open shader file error"),_T("´íÎó"),MB_OK);
	rewind(fp);
	fseek(fp,0,SEEK_END);
	count = ftell(fp);
	rewind(fp);
	fclose(fp);
	return count;
}

GLchar* CubeMapWithShader::ReadShaderSource(const char *fileName, int len)
{
	FILE *fp = fopen(fileName,"rt");
	if (!fp)
		MessageBox(NULL,_T("read shader file error"),_T("´íÎó"),MB_OK);
	GLchar *shader;
	shader = (GLchar *)malloc(sizeof(char)*(len+1));
	memset(shader,0,len+1);
	fseek(fp,0,SEEK_SET);
	fread(shader,sizeof(char),len,fp);
	fclose(fp);
	return shader;
}

int CubeMapWithShader::InstallShaders( GLuint &programObj,GLchar *Vertex, GLchar *Fragement )
{
	GLint vertCompiled,fragCompiled;
	GLint linked;

	//create
	vertexShaderObject = glCreateShader(GL_VERTEX_SHADER);
	fragementShaderObject = glCreateShader(GL_FRAGMENT_SHADER);
	
	//load
	glShaderSource(vertexShaderObject,1,(const char**)&Vertex,NULL);
	glShaderSource(fragementShaderObject,1,(const char**)&Fragement,NULL);
	
	// Load them. If fail clean up and return null
	// Vertex Program
	//if(gltLoadShaderFile(vertexFile, vertexShaderObject) == false)
	//{
	//	glDeleteShader(vertexShaderObject);
	//	glDeleteShader(fragementShaderObject);
	//	fprintf(stderr, "The shader at %s could ot be found.\n", vertexFile);
	//	return (GLuint)NULL;
	//}

	//// Fragment Program
	//if(gltLoadShaderFile(fragementFile, fragementShaderObject) == false)
	//{
	//	glDeleteShader(vertexShaderObject);
	//	glDeleteShader(fragementShaderObject);
	//	fprintf(stderr,"The shader at %s  could not be found.\n", fragementFile);
	//	return (GLuint)NULL;
	//}
	
	//compile
	glCompileShader(vertexShaderObject);
	glGetShaderiv(vertexShaderObject,GL_COMPILE_STATUS,&vertCompiled);
	glCompileShader(fragementShaderObject);
	glGetShaderiv(fragementShaderObject,GL_COMPILE_STATUS,&fragCompiled);

	if (!vertCompiled || !fragCompiled)
		MessageBox(NULL,_T("shader compile failed"),_T("´íÎó"),MB_OK);
    
	//create a program
	programObj = glCreateProgram();
	glAttachShader(programObj,vertexShaderObject);
	glAttachShader(programObj,fragementShaderObject);

	//link program
	glLinkProgram(programObj);

	glDeleteShader(vertexShaderObject);
	glDeleteShader(fragementShaderObject);  

	glGetProgramiv(programObj,GL_LINK_STATUS,&linked);
	if (!linked)
		MessageBox(NULL,_T("link program failed"),_T("´íÎó"),MB_OK);

	//get the compile fail info
	PrintShaderCompileInfo();
	return TRUE;
}

void CubeMapWithShader::RenderScene()
{
	DoInput();

	glPushMatrix();
	DrawCube();

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

	glPopMatrix();
}

void CubeMapWithShader::DrawCube()
{
	//glPushMatrix();
	//glTranslatef(0,0,z);
	glRotatef(xrot,1,0,0);
	glRotatef(yrot,0,1,0);

	glUseProgram(cubeProgramObject);

	glutSolidCube(20.0f);
	//glPopMatrix();
}

void CubeMapWithShader::DrawTorus()
{
	float InverseCamera[16];

	//glPushMatrix();
	glTranslatef(0,0,z);
	//glRotatef(xrot,1,0,0);
	//glRotatef(yrot,0,1,0);

	glUseProgram(reflectionProgramObject);

	glutSolidTorus(0.2,0.5,32,32);
	//glPopMatrix();
}
void CubeMapWithShader::DrawTeaport()
{
	float InverseCamera[16];
	
	//glPushMatrix();
	glTranslatef(0,0,z);
	//glRotatef(xrot,1,0,0);
	//glRotatef(yrot,0,1,0);

	glUseProgram(reflectionProgramObject);

	glutSolidTeapot(1.0f);
	//glPopMatrix();
}

void CubeMapWithShader::DrawSphere()
{
	float InverseCamera[16];
	
	//glPushMatrix();
	glTranslatef(0,0,z);
	//glRotatef(xrot,1,0,0);
	//glRotatef(yrot,0,1,0);

	glUseProgram(reflectionProgramObject);

	glEnable(GL_CULL_FACE);
	glutSolidSphere(1.0,52,26);
	glDisable(GL_CULL_FACE);

	//glPopMatrix();
}

void CubeMapWithShader::DoInput()
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

void CubeMapWithShader::PrintShaderCompileInfo()
{
	int infoLogLength;
	int charswritten;
	char *infolog;

	glGetShaderiv(vertexShaderObject,GL_INFO_LOG_LENGTH,&infoLogLength);
	infolog = (char *)malloc(infoLogLength);
	glGetShaderInfoLog(vertexShaderObject,infoLogLength,&charswritten,infolog);
	//MessageBox(NULL,(TCHAR *)infolog,_T("´íÎó"),MB_OK);
	cout<<infolog<<endl;

	glGetShaderiv(fragementShaderObject,GL_INFO_LOG_LENGTH,&infoLogLength);
	infolog = (char *)malloc(infoLogLength);
	glGetShaderInfoLog(fragementShaderObject,infoLogLength,&charswritten,infolog);
	//MessageBox(NULL,(TCHAR *)infolog,_T("´íÎó"),MB_OK);
	cout<<infolog<<endl;
}