#include "Camera.h"
#include <math.h>

// Vecteurs initiaux parfaitement alignés
vec3 forward = { 1.0f, 0.0f, 0.0f }; // On regarde vers l'avant
vec3 up = { 0.0f, 0.0f, 1.0f };
vec3 right = { 0.0f, 1.0f, 0.0f };
vec3 worldUp = { 0.0f, 0.0f, 1.0f }; // La gravité de la planète locale

float lastX = 1920.0f / 2.0f;
float lastY = 1080.0f / 2.0f;

void ProcessMouse(GLFWwindow *window)
{
    double cursorX, cursorY;
    glfwGetCursorPos(window, &cursorX, &cursorY);

    float xOffset = (float)cursorX - lastX;
    float yOffset = lastY - (float)cursorY;
    float sensitivity = 0.002f;

    // 1. Yaw : On tourne autour de la gravité locale (worldUp)
    glm_vec3_rotate(forward, -xOffset * sensitivity, worldUp);

    // 2. Pitch : On calcule l'angle pour bloquer avant la verticale pure
    float pitchAngle = yOffset * sensitivity;
    float currentDot = glm_vec3_dot(forward, worldUp);
    float currentAngle =
        acos(fmaxf(-1.0f, fminf(1.0f, currentDot))); // Sécurité anti-crash

    float limit = 0.05f; // Limite de 3 degrés environ
    if (currentAngle - pitchAngle < limit)
        pitchAngle = currentAngle - limit;
    if (currentAngle - pitchAngle > 3.14159f - limit)
        pitchAngle = currentAngle - (3.14159f - limit);

    // On calcule l'axe de rotation du Pitch (temporaire)
    vec3 tempRight;
    glm_vec3_cross(forward, worldUp, tempRight);
    glm_normalize(tempRight);

    // On applique le Pitch
    glm_vec3_rotate(forward, pitchAngle, tempRight);

    // 3. Orthonormalisation (Le secret pour ne plus jamais bloquer la caméra)
    glm_vec3_cross(forward, worldUp, right);
    glm_normalize(right);
    glm_vec3_cross(right, forward, up);
    glm_normalize(up);

    glfwSetCursorPos(window, 1920.0f / 2.0f, 1080.0f / 2.0f);
}