#pragma once

enum GLT_SHADER_ATTRIBUTE { GLT_ATTRIBUTE_VERTEX = 0, GLT_ATTRIBUTE_COLOR, GLT_ATTRIBUTE_NORMAL, 
GLT_ATTRIBUTE_TEXTURE0, GLT_ATTRIBUTE_TEXTURE1, GLT_ATTRIBUTE_TEXTURE2, GLT_ATTRIBUTE_TEXTURE3, 
GLT_ATTRIBUTE_LAST};

static GLenum cubeShaderFaces[6] = {
	GL_TEXTURE_CUBE_MAP_POSITIVE_X_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_X_EXT,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Y_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Y_EXT,
	GL_TEXTURE_CUBE_MAP_POSITIVE_Z_EXT,
	GL_TEXTURE_CUBE_MAP_NEGATIVE_Z_EXT
};

class CubeMapWithShader
{
public:
	CubeMapWithShader(void);
	~CubeMapWithShader(void);

private:
	int ShaderSize(const char *fileName);
	GLchar* ReadShaderSource(const char *fileName, int len);
	int InstallShaders(GLuint &programObj, GLchar *Vertex, GLchar *Fragement);
	void PrintShaderCompileInfo();
	void Init();
	void DoCubeMapping();

	void DrawCube();
	void DrawTorus();
	void DrawTeaport();
	void DrawSphere();
	void DoInput();

public:
	void SetupRC();
	void RenderScene();

private:
	GLuint vertexShaderObject;
	GLuint fragementShaderObject;
	GLuint reflectionProgramObject;
	GLuint cubeProgramObject;

	float xrot,yrot;
	float xspeed,yspeed;
	float z;
	int filter;

	GLuint cubeTexture;
};
