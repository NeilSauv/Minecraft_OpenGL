#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Headers/ShaderHeaders.h"

unsigned int vertexShader;
unsigned int fragmentShader;

unsigned int shaderProgram;

void ReadShader(const char* vertPath,const char* fragPath)
{
    const char* vertexShaderSource = Read(vertPath);
    const char* fragmentShaderSource = Read(fragPath);

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("error vertex shader");
        return;
    }
    // fragment shader
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("error fragment shader");
        return;

    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("error shader program");
        return;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
char* Read(const char* path)
{
    FILE* textfile;
    char* text;
    long numbytes;

    textfile = fopen(path, "r");
    if (!textfile)
    {
        printf("ReadPath is NULL");
        return NULL;
    }


    fseek(textfile, 0L, SEEK_END);
    numbytes = ftell(textfile);
    fseek(textfile, 0L, SEEK_SET);

    text = (char*)calloc(numbytes, sizeof(char));

    if (!text)
    {
        printf("Read calloc failed");
        return NULL;
    }

    int res = fread(text, sizeof(char), numbytes, textfile);
    if (!res)
    {
        printf("READ fread NULL");
        return NULL;
    }
    fclose(textfile);

    return text;
}

void CompileShader()
{
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);
}

void ClearShader()
{
    glDeleteProgram(shaderProgram);
}
