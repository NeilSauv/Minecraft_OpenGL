#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>
#include <math.h>

#define Debug

#include "main.h"

//Generators
#include "Generators/Noises/Headers/NoisesHeaders.h"
#include "Generators/Chunk/Headers/ChunkHeaders.h"
//Textures
#include "Textures/Headers/TextureHeaders.h"
//Shaders
#include "Shaders/Headers/ShaderHeaders.h"
//Utils
#include "Utils/Headers/UtilsHeaders.h"
//Player
#include "Player/Headers/PlayerHeaders.h"
//Physics
#include "Physics/Headers/PhysicsHeaders.h"


GLFWwindow* window;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void CreateWindow();


// settings
const unsigned int SCR_WIDTH = 1000;
const unsigned int SCR_HEIGHT = 1000;

int main()
{
#ifdef Debug
    OpenFile(); // Open print.txt for debbuging output
#endif
    
    InitNoiseStruct();

    InitHeightColorScheme();

    InitNoise(heightNoise);
    InitNoise(temperatureNoise);
    
    //Window
    CreateWindow();

    //Shaders
    ReadShader("ProjectFiles/Shaders/BasicVertices.vert", "ProjectFiles/Shaders/BasicColor.frag");

    // Init Regions
    InitRegion();

    //Mesh
    GenerateChunks();

    //Draw
    DrawNoise(temperatureNoise);

    //Texture
    ApplyTexture();

    glfwSetMouseButtonCallback(window, mouse_callback);

    mat4 projection = {{0.0, 0.0, 0.0}};
    glm_perspective(glm_rad(90.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f, projection);

    glfwSetCursorPos(window, 1000 / 2, 1000 / 2);
    
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        
        //Deltatime
        UpdateDeltaTime();

        //Inputs
        ProcessMoves(window);
        ProcessMouse(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, selectedTexture);
        glUniform1i(glGetUniformLocation(shaderProgram, "selectedTexture"), 1);

        
        //glUniform1i(glGetUniformLocation(shaderProgram, "atlas"), 0);

        //glUniform1iv(glGetUniformLocation(shaderProgram, "rendering"), ChunkSize*ChunkSize*ChunkSize, &rendering[0]);

        //glUniform1i(glGetUniformLocation(shaderProgram, "ChunkSize"), ChunkSize);
        
        mat4 view;
        glm_mat4_identity(view);

        vec3 cameraTarget = { 0.0, 0.0, 0.0 };
        glm_vec3_add(cameraPos, forward, cameraTarget);
        glm_lookat(cameraPos, cameraTarget, up, view);

        mat4 model;
        glm_mat4_identity(model);

        glm_perspective(glm_rad(45.0f), SCR_WIDTH / SCR_HEIGHT, 0.1f, 10000.0, projection);

        //Compile Shader
        CompileShader();
        
        //Uniform vars
        unsigned int projectionLoc = glGetUniformLocation(shaderProgram, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE,(const GLfloat *) projection);
        unsigned int modelLoc = glGetUniformLocation(shaderProgram, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE,(const GLfloat *)  model);
        unsigned int viewLoc = glGetUniformLocation(shaderProgram, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE,(const GLfloat *) view);
        
        Update();
        
        DrawChunk();
        
        //Swap Buffers
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    //Clear
    FreeColorSchemes();
    ClearChunk();
    ClearTexture();
    ClearShader();

    glfwTerminate();

    FreeStruct();
    
#ifdef Debug
    CloseFile();
#endif

    return 0;
}

void CreateWindow()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Minecraft", NULL, NULL);

    // glfw window creation
    // --------------------
    if (window == NULL)
    {
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return;
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}