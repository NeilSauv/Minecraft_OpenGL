#ifndef FILE_UTILS
#define FILE_UTILS

#include <cglm/cglm.h>

void OpenFile();

void WriteLine(char string[]);

void WriteLinei(int i);

float WriteLinef(float f);

void PrintVec3(vec3 vec);

void PrintVec4(vec4 vec);

void PrintMat4(mat4 mat);

void PrintArr3(float arr[]);

void CloseFile();

#endif // !FILE_UTILS

