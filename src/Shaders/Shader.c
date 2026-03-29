#include "Shader.h"

#include <windows.h>

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>

unsigned int vertexShader;
unsigned int fragmentShader;

unsigned int shaderProgram;

void ReadShader(const char *vertPath, const char *fragPath)
{
    const char *vertexShaderSource = Read(vertPath);
    const char *fragmentShaderSource = Read(fragPath);

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
        printf("VERTEX SHADER COMPILE ERROR:\n%s\n", infoLog);
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
        printf("FRAGMENT SHADER COMPILE ERROR:\n%s\n", infoLog);
        return;
    }
    // link shaders
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("SHADER LINK ERROR:\n%s\n", infoLog);
        return;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

char *Read(const char *path)
{
    HANDLE hFile = CreateFileA(path, GENERIC_READ, FILE_SHARE_READ, NULL,
                               OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        printf("ReadPath is NULL");
        return NULL;
    }

    DWORD numbytes = GetFileSize(hFile, NULL);

    char *text = (char *)calloc(numbytes + 1, sizeof(char));
    if (!text)
    {
        printf("Read calloc failed");
        CloseHandle(hFile);
        return NULL;
    }

    DWORD bytesRead;
    BOOL res = ReadFile(hFile, text, numbytes, &bytesRead, NULL);
    if (!res || bytesRead == 0)
    {
        printf("READ ReadFile failed");
        CloseHandle(hFile);
        return NULL;
    }
    CloseHandle(hFile);

    return text;
}

void CompileShader()
{
    glUseProgram(shaderProgram);
}

void ClearShader()
{
    glDeleteProgram(shaderProgram);
}
