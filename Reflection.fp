// Reflection Shader
// Fragment Shader
#version 130

uniform vec3  BaseColor;
uniform float MixRatio;

uniform samplerCube EnvMap;

varying vec3  ReflectDir;
varying float LightIntensity;

void main(void)
{ 
	// Look up environment map value in cube map
    vec3 envColor = vec3(textureCube(EnvMap, ReflectDir));
    
    // Add lighting to base color and mix
    vec3 base = LightIntensity * BaseColor;
    envColor  = mix(envColor, base, MixRatio);

    gl_FragColor = vec4(envColor, 1.0);
}
    