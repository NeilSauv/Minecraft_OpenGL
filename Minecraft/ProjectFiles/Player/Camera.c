#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include "../main.h"
#include "../Utils/Headers/FileUtils.h"

vec3 forward = { 0.0f, 0.0f, -1.0f };
vec3 up = { 0.0f, 1.0f, 0.0f };
vec3 right = { 1.0, 0.0f, 0.0f };

float yaw = 270.0f;
float pitch = 0;

float lastX = 1000 / 2;
float lastY = 1000 / 2;

void ProcessMouse(GLFWwindow* window)
{
    double cursorX = 0;
    double cursorY = 0;

    glfwGetCursorPos(window, &cursorX, &cursorY);

    float xOffset = cursorX - lastX;
    float yOffset = lastY - cursorY;

    float sensitivity = 0.15f;

    yaw += xOffset * sensitivity;
    pitch += yOffset * sensitivity;

    if (pitch > 89.999f)
        pitch = 89.999f;
    if (pitch < -89.999f)
        pitch = -89.999f;

    if (yaw > 360)
        yaw = 0;
    else if (yaw < 0)
        yaw = 360;

    vec3 direction =
    {
        cos(glm_rad(yaw)) * cos(glm_rad(pitch)),
        sin(glm_rad(pitch)),
        sin(glm_rad(yaw)) * cos(glm_rad(pitch))
    };



    glm_normalize_to(direction, forward);
    glfwSetCursorPos(window, 1000 / 2, 1000 / 2);
}