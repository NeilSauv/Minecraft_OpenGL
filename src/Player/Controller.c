#include "Controller.h"
#include <Physics/Collisions.h>
#include <Player/Camera.h>
#include <Player/Destroy.h>
#include <Utils/FileUtils.h>
#include <Utils/TimeUtils.h>
#include <cglm/cglm.h>
#include "GLFW/glfw3.h"

vec3 cameraPos = { 0.0f, 0.0f, 650.0f };

bool groundCheck = false;
bool jumped = false;

float failSpeed = 0.0f;
float increaseFail = 25.0f; // Puissance de la gravité
float actualFailSpeed = 0.0f;
float speed = 4.16f;

bool godMod = true; // God mode activé par défaut (Appuyez sur G pour désactiver
                    // et tomber !)

void ProcessInput(GLFWwindow *window);
void Falling();

void ProcessMoves(GLFWwindow *window)
{
    // 1. D'ABORD : On déplace le joueur (horizontalement)
    ProcessInput(window);

    // 2. ENSUITE : On applique la gravité s'il n'est pas en God Mode
    if (!godMod)
        Falling();

    // 3. SEULEMENT APRES : On calcule la nouvelle orientation de la caméra
    // Cela garantit 0 saccade, car la caméra s'adapte à la position finale
    // exacte !
    vec3 newUp;
    glm_vec3_copy(cameraPos, newUp);
    glm_normalize(newUp);

    vec3 rotAxis;
    glm_vec3_cross(worldUp, newUp, rotAxis);
    float dotTheta = glm_vec3_dot(worldUp, newUp);

    if (glm_vec3_norm(rotAxis) > 0.000001f)
    {
        glm_normalize(rotAxis);
        float angle = acosf(fmaxf(-1.0f, fminf(1.0f, dotTheta)));

        glm_vec3_rotate(forward, angle, rotAxis);
        glm_vec3_rotate(up, angle, rotAxis);
        glm_vec3_rotate(right, angle, rotAxis);
    }

    glm_vec3_copy(newUp, worldUp);

    // Orthonormalisation (Protège des erreurs mathématiques SANS détruire le
    // pitch/regard)
    glm_normalize(forward);
    glm_vec3_cross(forward, worldUp, right);
    glm_normalize(right);
    glm_vec3_cross(right, forward, up);
    glm_normalize(up);
}

void ProcessInput(GLFWwindow *window)
{
    const float runSpeed = 150.0f;
    const float walkSpeed = 30.0f;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, 1);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
        speed = runSpeed;
    else
        speed = walkSpeed;

    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        godMod = !godMod; // Bascule entre le mode Vol et le mode Gravité

    vec3 moveDir = { 0.0f, 0.0f, 0.0f };

    // Déplacement plaqué au sol (tangent à la planète)
    vec3 moveForward;
    glm_vec3_cross(worldUp, right, moveForward);
    glm_normalize(moveForward);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        glm_vec3_add(moveDir, moveForward, moveDir);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        glm_vec3_sub(moveDir, moveForward, moveDir);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        glm_vec3_sub(moveDir, right, moveDir);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        glm_vec3_add(moveDir, right, moveDir);

    if (glm_vec3_norm(moveDir) > 0.0f)
        glm_normalize(moveDir);

    vec3 velocity;
    glm_vec3_scale(moveDir, speed * deltaTime, velocity);

    if (godMod)
    {
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            vec3 upMove;
            glm_vec3_scale(worldUp, speed * deltaTime, upMove);
            glm_vec3_add(velocity, upMove, velocity);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
        {
            vec3 downMove;
            glm_vec3_scale(worldUp, -speed * deltaTime, downMove);
            glm_vec3_add(velocity, downMove, velocity);
        }
    }
    else // Mode Survie !
    {
        // Système de SAUT
        if (groundCheck && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        {
            jumped = true;
            actualFailSpeed =
                -15.0f; // Une vitesse négative nous pousse vers le haut
            groundCheck = false;
        }
    }

    if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glm_vec3_add(cameraPos, velocity, cameraPos);
}

void Falling()
{
    vec3 target;
    vec3 fallVec;

    // actualFailSpeed positif = on tombe. Négatif = on saute.
    glm_vec3_scale(worldUp, actualFailSpeed * deltaTime, fallVec);
    glm_vec3_sub(cameraPos, fallVec,
                 target); // On soustrait worldUp pour tomber vers le centre

    if (MovesCollisions(target)) // S'il n'y a pas de mur/sol
    {
        glm_vec3_copy(target, cameraPos);
        actualFailSpeed += increaseFail * deltaTime; // Accélération de la chute
        groundCheck = false;
    }
    else // On a touché le sol
    {
        groundCheck = true;
        if (actualFailSpeed
            > 0.0f) // On réinitialise la vitesse uniquement si on tombait
        {
            actualFailSpeed = failSpeed;
        }
        jumped = false;
    }
}

void mouse_callback(GLFWwindow *window, int button, int action, int mods)
{
    if (!window && !mods)
        return;
}