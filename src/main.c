#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#define NOMINMAX
#include <windows.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <stdio.h>

#include <Generators/Chunk/ChunkManager.h>
#include <Generators/Chunk/Region.h>
#include <Generators/Noises/NoiseStruct.h>
#include <Generators/Noises/SimplexNoise.h>
#include <Textures/Block.h>
#include <Player/Camera.h>
#include <Player/Controller.h>
#include <Shaders/Shader.h>
#include <Textures/ColorMap.h>
#include <Utils/FileUtils.h>
#include <Utils/TimeUtils.h>

const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;
GLFWwindow *window;

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void CreateGameWindow();

#define CHECK(cond, msg)                                                       \
    if (!(cond))                                                               \
    {                                                                          \
        printf("ERROR: %s\n", msg);                                            \
        exit(1);                                                               \
    }

void CheckGLError(const char *step)
{
    GLenum err;
    while ((err = glGetError()) != GL_NO_ERROR)
    {
        printf("GL ERROR at %s: %d\n", step, err);
    }
}

int main()
{
    OpenLogFile();
    printf("InitNoiseStruct\n");
    InitNoiseStruct();
    CHECK(heightNoise != NULL, "heightNoise NULL");

    printf("Init Simplex\n");
    open_simplex_noise(200, heightNoise);

    printf("InitHeightColorScheme\n");
    InitHeightColorScheme();

    InitBlockPattern(heightNoise);

    // Création de la fenêtre
    CreateGameWindow();

    // Shaders
    ReadShader("Assets/Shaders/BasicVertices.vert",
               "Assets/Shaders/BasicColor.frag");
    CHECK(shaderProgram != 0, "SHADER PROGRAM INVALID");

    InitRegion();
    GenerateChunks(); // Génération du maillage planétaire

    glfwSetMouseButtonCallback(window, mouse_callback);
    mat4 projection = { { 0.0, 0.0, 0.0 } };
    glm_perspective(glm_rad(75.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 1.0f,
                    10000.0f, projection);

    glfwSetCursorPos(window, SCR_WIDTH / 2.f, SCR_HEIGHT / 2.f);

    // Paramètres 3D cruciaux !
    glEnable(GL_DEPTH_TEST); // Active la profondeur 3D

    double lastTime = glfwGetTime();
    int nbFrames = 0;

    CheckGLError("Before Render Loop");

    // Boucle de rendu
    while (!glfwWindowShouldClose(window))
    {
        UpdateDeltaTime();
        double currentTime = glfwGetTime();
        nbFrames++;
        if (currentTime - lastTime >= 1.0)
        {
            nbFrames = 0;
            lastTime = currentTime;
        }

        ProcessMoves(window);
        ProcessMouse(window);

        // Couleur du ciel (espace)
        glClearColor(0.1f, 0.15f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4 view;
        glm_mat4_identity(view);

        vec3 cameraTarget = { 0.0, 0.0, 0.0 };
        glm_vec3_add(cameraPos, forward, cameraTarget);
        glm_lookat(cameraPos, cameraTarget, up, view);

        mat4 model;
        glm_mat4_identity(model);

        CompileShader();

        unsigned int projectionLoc =
            glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,
                           (const GLfloat *)projection);

        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat *)model);

        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const GLfloat *)view);

        Update();
        DrawChunk();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    CloseFile();
    FreeColorSchemes(heightNoise);
    ClearChunk();
    ClearShader();
    glfwTerminate();

    return 0;
}

void CreateGameWindow()
{
    CHECK(glfwInit(), "GLFW INIT FAILED");
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window =
        glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Planete Low-Poly", NULL, NULL);

    if (window == NULL)
    {
        printf("Failed to create GLFW window");
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        printf("Failed to initialize GLAD");
        return;
    }
    CheckGLError("GLAD INIT");
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
    if (!window)
        return;
    glViewport(0, 0, width, height);
}