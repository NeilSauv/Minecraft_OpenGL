#ifndef CONTROLLER
#define CONTROLLER

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <cglm/cglm.h>

extern vec3 cameraPos;
extern bool update;
extern bool jumped;

void ProcessMoves(GLFWwindow *window);

void mouse_callback(GLFWwindow *window, int button, int action, int mods);

#endif // !CONTROLLER
