
#ifndef _INCLUDEHEAD_
#define _INCLUDEHEAD_

#pragma comment( lib, "opengl32.lib" )				
#pragma comment( lib, "glu32.lib" )	
#pragma comment( lib, "glut32.lib")
#pragma comment( lib, "glaux.lib")
#pragma comment( lib, "vfw32.lib" )	

#include <gl/glut.h>
#include <gl/glaux.h>
#include <stdio.h>
#include <gl/glu.h>
#include <windows.h>
#include <math.h>
#include <stdarg.h>
#include <assert.h>



#define BITMAP_ID   0x4D42    // the universal bitmap ID

#define PI		3.1415926
#define CUBE_MAP_SIZE 256

#define KEY_DOWN(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code)((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)
#define RAND_COORD(x)   ((float)rand()/RAND_MAX * (x))
#define FRAND   (((float)rand()-(float)rand())/RAND_MAX)

#endif