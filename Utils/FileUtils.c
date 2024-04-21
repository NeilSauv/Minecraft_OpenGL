#include <stdio.h>
#include <cglm/cglm.h>

#include "FileUtils.h"

FILE* file;
char* path = "print.txt";

void OpenFile();
void CloseFile();
void PrintVec3(vec3 vec);

void OpenFile()
{
    file = fopen(path, "w");

    if (file == NULL)
    {
        printf("File not found");
        exit(1);
    }
}

void WriteLine(char string[])
{
    fputs(string, file);
}

void WriteLinei(int i) 
{
    fprintf(file, "%d\n", i);
}

float WriteLinef(float f)
{
    fprintf(file, "%f\n", f);
    return f;
}

float last[3] = {-0.42, -0.42, -0.42};

void PrintVec3(vec3 vec)
{
    if (last[0] != vec[0] || last[1] != vec[1] || last[2] != vec[2])
    {
        glm_vec3_print(vec, file);
    }

    last[0] = vec[0];
    last[1] = vec[1];
    last[2] = vec[2];
}

void PrintVec4(vec4 vec)
{
    glm_vec4_print(vec, file);
}

void PrintMat4(mat4 mat)
{
    glm_mat4_print(mat, file);
}

void PrintArr3(float arr[])
{
    vec3 vec = { arr[0], arr[1], arr[2] };
    PrintVec3(vec);
}


void CloseFile()
{
    fclose(file);
}
