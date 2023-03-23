#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

#include "../Player/Headers/PlayerHeaders.h"
#include "../Utils/Headers/UtilsHeaders.h"
#include "../Generators/Chunk/Headers/ChunkHeaders.h"
#include "../Physics/Headers/PhysicsHeaders.h"

vec3 cameraPos = { 0.0f, 100.0f, 0.0f };

bool groundCheck = false;
float jumpForce = 1.5f;

bool jumped = false;
float jumpHeight = 0.0f;
float jumpIncrease = 10.0f;

float failSpeed = 1.3f;
float increaseFail = 10.0f;
float actualFailSpeed = 1.5f;

float speed = 2.5f;

void ProcessInput(GLFWwindow* window);
void Falling(vec3 cameraPos);

bool godMod = true;

void ProcessMoves(GLFWwindow* window)
{
    ProcessInput(window);
    if(!godMod)
        Falling(cameraPos);
}

void ProcessInput(GLFWwindow* window)
{
    const float runSpeed = 50.0f;
    const float walkSpeed = 15.0f;

    vec3 forwardDir = { forward[0], 0, forward[2]};
    vec3 targetCameraPos = { 0.0, 0.0, 0.0 };

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    vec3 iddentity = { 1.0f, 1.0f, 1.0f };
    vec3 cameraSpeed;
    glm_vec3_scale(iddentity, speed * deltaTime, cameraSpeed);
    vec3 temp = { 0.0, 0.0, 0.0 };


    if (jumped)
    {
        if (cameraPos[1] >= jumpHeight)
        {
            groundCheck = false;
            jumped = false;
        }
        else
        {
            vec3 jump = { 0.0, 0.0, 0.0 };
            glm_vec3_scale(up, jumpIncrease * deltaTime, jump);
            glm_vec3_add(cameraPos, jump, cameraPos);
        }
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speed = runSpeed;
    else
        speed = walkSpeed;

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        godMod = !godMod;

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS && ((groundCheck && !jumped) || godMod))
    {
        if (!godMod && groundCheck && !jumped)
        {
            vec3 jump = { 0.0, 0.0, 0.0 };
            glm_vec3_scale(up, jumpForce, jump);
            jumpHeight = cameraPos[1] + jump[1];
            jumped = true;
            groundCheck = false;
        }
        else if(godMod)
            glm_vec3_muladd(cameraSpeed, up, targetCameraPos);

    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS) //&& groundCheck //&& !jumped)
    {
        glm_vec2_negate_to(up, temp);
        glm_vec3_muladd(cameraSpeed, temp, targetCameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    {
        glm_vec3_muladd(cameraSpeed, forwardDir, targetCameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    {
        glm_vec3_mul(cameraSpeed, forwardDir, temp);
        glm_vec3_sub(targetCameraPos, temp, targetCameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        glm_vec3_cross(forwardDir, up, temp);
        glm_normalize(temp);
        glm_vec3_mul(temp, cameraSpeed, temp);
        glm_vec3_sub(targetCameraPos, temp, targetCameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    {
        glm_vec3_cross(forwardDir, up, temp);
        glm_normalize(temp);
        glm_vec3_muladd(temp, cameraSpeed, targetCameraPos);
    }
    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        PrintVec3(cameraPos);
    }

    glm_vec3_add(cameraPos, targetCameraPos, targetCameraPos);

    if(MovesCollisions(targetCameraPos))
        glm_vec3_scale(targetCameraPos, 1, cameraPos);
}

void mouse_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        DestroyBlock();
    }
}

void Falling(vec3 cameraPos)
{


    int x = round(cameraPos[0]);
    int z = round(cameraPos[2]);

    vec3 target = { x,cameraPos[1]-0.01, z };
    if (MovesCollisions(target) && !jumped)
    {
        vec3 temp = { 0.0, 0.0, 0.0 };
        glm_vec2_negate_to(up, temp);
        glm_vec3_scale(temp, actualFailSpeed * deltaTime, temp);
        glm_vec3_add(cameraPos, temp, cameraPos);
        actualFailSpeed += increaseFail * deltaTime;
        groundCheck = false;
    }
    else
    {
        groundCheck = true;
        actualFailSpeed = failSpeed;
    }

}
