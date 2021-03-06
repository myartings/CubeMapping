// Skybox Shader
// Fragment Shader
#version 130

out vec4 vFragColor;

uniform samplerCube  cubeMap;

varying vec3 vVaryingTexCoord;

void main(void)
{ 
    vFragColor = texture(cubeMap, vVaryingTexCoord);
}
    