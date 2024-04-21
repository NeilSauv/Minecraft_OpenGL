#include "TextureSet.h"

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <stb/stb_image.h>
#include <stdio.h>
#include <stdlib.h>

unsigned int texture1;
unsigned int selectedTexture;

void ApplyTexture()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D,
                  texture1); // all upcoming GL_TEXTURE_2D operations now have
                             // effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT); // set texture wrapping to GL_REPEAT (default
                                // wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // load image, create texture and generate mipmaps
    int width, height, nrChannels;

    // The FileSystem::getPath(...) is part of the GitHub repository so we can
    // find files on any IDE/platform; replace it with your own image path.
    unsigned char *data = stbi_load("Textures/Pictures/atlas.png", &width,
                                    &height, &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        // glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, 16, 16, GL_RGBA,
        // GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture");
        return;
    }
    stbi_image_free(data);

    // glEnable(GL_BLEND);
    // glBlendEquation(GL_FUNC_ADD);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glGenTextures(1, &selectedTexture);
    glBindTexture(GL_TEXTURE_2D,
                  selectedTexture); // all upcoming GL_TEXTURE_2D operations now
                                    // have effect on this texture object
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_EXTENSIONS); // set texture wrapping to GL_REPEAT
                                    // (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_EXTENSIONS);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // The FileSystem::getPath(...) is part of the GitHub repository so we can
    // find files on any IDE/platform; replace it with your own image path.
    data = stbi_load("Textures/Pictures/selected.png", &width, &height,
                     &nrChannels, 0);

    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                     GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf("Failed to load texture");
        return;
    }
    stbi_image_free(data);
}

unsigned char *ReadBPM(char *fileName)
{
    int width, height, channels;
    unsigned char *arr = NULL;
    arr = stbi_load(fileName, &width, &height, &channels, 0);
    if (arr == NULL)
    {
        printf("Error in loading the image\n");
        exit(1);
    }

    return arr;
}

void ClearTexture()
{
    glDeleteTextures(1, &texture1);
    // glDeleteTextures(1, &selectedTexture);
}
