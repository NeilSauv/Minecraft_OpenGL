#include "FileUtils.h"

#include <windows.h>

#include <cglm/cglm.h>
#include <stdio.h>
#include <string.h>

HANDLE file = INVALID_HANDLE_VALUE;
static const char *path = "print.txt";

void OpenLogFile();
void CloseFile();
void PrintVec3(vec3 vec);

static void WriteStr(const char *str)
{
    DWORD written;
    WriteFile(file, str, (DWORD)strlen(str), &written, NULL);
}

void OpenLogFile()
{
    file = CreateFileA(path, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS,
                       FILE_ATTRIBUTE_NORMAL, NULL);

    if (file == INVALID_HANDLE_VALUE)
    {
        printf("File not found\n");
        exit(1);
    }
}

void WriteLine(char string[])
{
    WriteStr(string);
}

void WriteLinei(int i)
{
    char buf[32];
    sprintf(buf, "%d\n", i);
    WriteStr(buf);
}

float WriteLinef(float f)
{
    char buf[32];
    sprintf(buf, "%f\n", f);
    WriteStr(buf);
    return f;
}

float last[3] = { -0.42f, -0.42f, -0.42f };

void PrintVec3(vec3 vec)
{
    if (last[0] != vec[0] || last[1] != vec[1] || last[2] != vec[2])
    {
        char buf[64];
        sprintf(buf, "Vec3(%.2f, %.2f, %.2f)\n", vec[0], vec[1], vec[2]);
        WriteStr(buf);
    }

    last[0] = vec[0];
    last[1] = vec[1];
    last[2] = vec[2];
    FlushFileBuffers(file);
}

void PrintVec4(vec4 vec)
{
    char buf[96];
    sprintf(buf, "Vec4(%.2f, %.2f, %.2f, %.2f)\n", vec[0], vec[1], vec[2],
            vec[3]);
    WriteStr(buf);
}

void PrintMat4(mat4 mat)
{
    char buf[256];
    sprintf(buf,
            "Mat4(\n  %.2f %.2f %.2f %.2f\n  %.2f %.2f %.2f %.2f\n"
            "  %.2f %.2f %.2f %.2f\n  %.2f %.2f %.2f %.2f\n)\n",
            mat[0][0], mat[0][1], mat[0][2], mat[0][3], mat[1][0], mat[1][1],
            mat[1][2], mat[1][3], mat[2][0], mat[2][1], mat[2][2], mat[2][3],
            mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
    WriteStr(buf);
}

void PrintArr3(float arr[])
{
    vec3 vec = { arr[0], arr[1], arr[2] };
    PrintVec3(vec);
}

void CloseFile()
{
    CloseHandle(file);
    file = INVALID_HANDLE_VALUE;
}
