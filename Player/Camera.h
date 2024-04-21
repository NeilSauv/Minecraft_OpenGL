#ifndef CAMERA
#define CAMERA

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

extern vec3 forward;
extern vec3 up;
extern vec3 right;

void ProcessMouse(GLFWwindow *window);

#endif