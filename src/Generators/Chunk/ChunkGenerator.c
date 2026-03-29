#include "ChunkGenerator.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <Generators/Noises/NoiseStruct.h>
#include <Textures/DrawNoise.h>
#include <Textures/ColorMap.h>
#include <Textures/BlockDef.h>
#include <stdlib.h>
#include <stdio.h>

// Allocation de nos tableaux globaux (déclarés dans le .h)
float chunkVertices[VERTICES_PER_CHUNK * 6];
unsigned int chunkIndices[INDICES_PER_CHUNK];

unsigned int VBO[ChunkView * ChunkView * 4];
unsigned int VAO[ChunkView * ChunkView * 4];
unsigned int EBO[ChunkView * ChunkView * 4];

void CreateChunk(int xAxis, int yAxis, int zAxis, int i, bool destroyBlock)
{
    // Sur une heightmap, on ne gère que le niveau de la surface (y=0)
    if (yAxis != 0)
        return;

    int vertexIndex = 0;

    // 1. GÉNÉRATION DES SOMMETS (VERTICES)
    // On boucle jusqu'à <= ChunkSize (donc 17 points de large)
    for (int z = 0; z <= ChunkSize; z++)
    {
        for (int x = 0; x <= ChunkSize; x++)
        {
            BlockInfoStruct blockInfo;

            // On interroge directement le bruit mathématique pour cette
            // coordonnée !
            float heightF = GetSingleNoiseVal(xAxis + x, zAxis + z, &blockInfo,
                                              heightNoise);
            float height = heightF * ChunkHeight;

            // Rendre l'eau parfaitement plate
            if (height < WaterLevel)
            {
                height = WaterLevel;
            }

            // Récupérer la couleur assignée (herbe, sable, neige...) depuis le
            // ColorMap
            RGB *color = GetBlockColor(&blockInfo, heightNoise);
            float r = 1.0f, g = 1.0f, b = 1.0f; // Blanc par défaut
            if (color)
            {
                r = color->red / 255.0f;
                g = color->green / 255.0f;
                b = color->blue / 255.0f;
            }

            // --- Enregistrement du sommet dans le tableau ---

            // Position 3D (X, Y, Z)
            chunkVertices[vertexIndex++] = (float)(xAxis + x);
            chunkVertices[vertexIndex++] = height;
            chunkVertices[vertexIndex++] = (float)(zAxis + z);

            // Couleur du point (R, G, B)
            chunkVertices[vertexIndex++] = r;
            chunkVertices[vertexIndex++] = g;
            chunkVertices[vertexIndex++] = b;
        }
    }

    // 2. GÉNÉRATION DES TRIANGLES (INDICES)
    int indexCount = 0;
    int rowSize = ChunkSize + 1; // 17 points par ligne

    // Pour lier les triangles, on boucle sur les 16x16 "cases"
    for (int z = 0; z < ChunkSize; z++)
    {
        for (int x = 0; x < ChunkSize; x++)
        {
            // On récupère l'index des 4 coins de la case actuelle
            int topLeft = z * rowSize + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * rowSize + x;
            int bottomRight = bottomLeft + 1;

            // Premier triangle de la case (Haut-Gauche, Bas-Gauche, Haut-Droit)
            chunkIndices[indexCount++] = topLeft;
            chunkIndices[indexCount++] = bottomLeft;
            chunkIndices[indexCount++] = topRight;

            // Deuxième triangle de la case (Haut-Droit, Bas-Gauche, Bas-Droit)
            chunkIndices[indexCount++] = topRight;
            chunkIndices[indexCount++] = bottomLeft;
            chunkIndices[indexCount++] = bottomRight;
        }
    }

    // 3. ENVOI À LA CARTE GRAPHIQUE (VBO / VAO / EBO)
    glBindVertexArray(VAO[i]);

    // Envoi des sommets
    glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(chunkVertices), chunkVertices,
                 GL_STATIC_DRAW);

    // Envoi des indices (triangles)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[i]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(chunkIndices), chunkIndices,
                 GL_STATIC_DRAW);

    // Explication du layout à OpenGL pour "chunkVertices" :

    // Attribut 0 : Position (3 floats). Commence au tout début (offset 0). Il y
    // a 6 floats en tout par point.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)0);
    glEnableVertexAttribArray(0);

    // Attribut 1 : Couleur (3 floats). Commence après les 3 floats de la
    // position.
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float),
                          (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
}

void Draw(int i)
{
    int n = 0;
    while (n < i)
    {
        glBindVertexArray(VAO[n]);
        // Nous ne faisons plus de glDrawElementsInstanced !
        // On dessine simplement les triangles normaux du maillage.
        glDrawElements(GL_TRIANGLES, INDICES_PER_CHUNK, GL_UNSIGNED_INT, 0);
        n++;
    }
}