#ifndef Shader
#define Shader

#include <stdio.h>

extern unsigned int vertexShader;
extern unsigned int fragmentShader;

extern unsigned int shaderProgram;

void ReadShader(const char* vertPath,const char* fragPath);

char* Read(const char* path);

void CompileShader();

void ClearShader();

#endif
