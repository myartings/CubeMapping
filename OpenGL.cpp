#include "stdafx.h"
#include "CubeMapping.h"
#include "OpenGL.h"
#include "CubeMapWithShader.h"
#include "math3d.h"

CubeMapping *cm;
CubeMapWithShader *cms;

OpenGL::OpenGL()
{	
	cm = NULL;
	cms = NULL;
}

OpenGL::~OpenGL()
{
	if (cm != NULL)
	{
		delete cm;
		cm = NULL;
	}

	if (cms != NULL)
	{
		delete cms;
		cms = NULL;
	}
}

bool OpenGL::Init()
{
	glewInit();
	if (!glewIsSupported("GL_VERSION_2_0 GL_VERSION_1_5 GL_ARB_multitexture GL_ARB_vertex_buffer_object")) {
		fprintf(stderr, "Required OpenGL extensions missing.");
		exit(-1);
	}

	glShadeModel(GL_SMOOTH);	
	glClearColor(0,0,0,0.5f);
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	
	//cpu
	cm=new CubeMapping();
	
	//gpu, with glsl
	//cms = new CubeMapWithShader();
	//cms->SetupRC();
	return true;

}

void OpenGL::Reshape(int width,int height)
{
	glViewport(0,0,width,height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(35.0f,width/(float)height,1.0f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}	

void OpenGL::Display()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	//cpu
	cm->DoDraw();
	
	//gpu, with glsl
	//cms->RenderScene();
}

