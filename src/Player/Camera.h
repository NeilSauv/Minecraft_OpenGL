#ifndef CAMERA
#define CAMERA

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

extern vec3 forward;
extern vec3 up;
extern vec3 right;
extern vec3 worldUp; // NOUVEAU: La gravité de la planète

void ProcessMouse(GLFWwindow *window);

#endif