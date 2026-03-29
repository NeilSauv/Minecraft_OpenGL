#include "TimeUtils.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

float deltaTime = 0.0f;
float lastFrame = 0.0f;

void UpdateDeltaTime()
{
    float currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame;

    if (deltaTime > 0.1f)
        deltaTime = 0.1f;

    lastFrame = currentFrame;
}
