#include "ColorMap.h"

#include <Generators/Noises/NoiseStruct.h>
#include <stdio.h>
#include <stdlib.h>

#include "Textures/Block.h"
#include "Textures/BlockDef.h"

void AddColorScheme(enum BlockTypeEnum block, float limit, int red, int green,
                    int blue, ColorScheme *colorScheme)
{
    struct RGB *color = (struct RGB *)malloc(sizeof(struct RGB));
    if (!color)
    {
        printf("color malloc failed\n");
        return;
    }

    color->red = red;
    color->green = green;
    color->blue = blue;

    Scheme *scheme = malloc(sizeof(Scheme));
    if (!scheme)
    {
        free(color);
        printf("scheme malloc failed\n");
        return;
    }
    scheme->limit = limit;
    scheme->color = color;
    scheme->block = block;
    scheme->next = NULL;

    if (!colorScheme->size)
        colorScheme->begin = scheme;
    else
        colorScheme->end->next = scheme;

    colorScheme->end = scheme;
    colorScheme->size++;
}

void InitHeightColorScheme()
{
    ColorScheme *heightColorScheme = heightNoise->colorScheme;

    heightColorScheme->begin = NULL;
    heightColorScheme->end = NULL;
    heightColorScheme->size = 0;
    heightColorScheme->useBlock = 1;

    AddColorScheme(BLOCK_SNOW, 0.85f, 255, 255, 255, heightColorScheme);
    AddColorScheme(BLOCK_STONE, 0.80f, 163, 163, 163, heightColorScheme);
    AddColorScheme(BLOCK_GRASS_BLOCK, 0.03f, 0, 255, 0, heightColorScheme);
    AddColorScheme(BLOCK_SAND, 0.00f, 200, 255, 0, heightColorScheme);
    AddColorScheme(BLOCK_WATER_FLOW, heightNoise->minNoiseHeight, 0, 0, 255,
                   heightColorScheme);

    ColorScheme *temperatureColorScheme = temperatureNoise->colorScheme;

    temperatureColorScheme->begin = NULL;
    temperatureColorScheme->end = NULL;
    temperatureColorScheme->size = 0;
    temperatureColorScheme->useBlock = 0;

    AddColorScheme(BLOCK_AIR, 1, 173, 0, 0, temperatureColorScheme);
    AddColorScheme(BLOCK_AIR, 0.7f, 255, 70, 0, temperatureColorScheme);
    AddColorScheme(BLOCK_AIR, 0.6f, 255, 120, 0, temperatureColorScheme);
    AddColorScheme(BLOCK_AIR, 0.5f, 255, 182, 0, temperatureColorScheme);
    AddColorScheme(BLOCK_AIR, 0.4f, 246, 250, 209, temperatureColorScheme);
    AddColorScheme(BLOCK_AIR, 0.3f, 209, 229, 250, temperatureColorScheme);
    AddColorScheme(BLOCK_AIR, 0.2f, 87, 120, 255, temperatureColorScheme);
    AddColorScheme(BLOCK_AIR, 0.1f, 34, 0, 119, temperatureColorScheme);

    ColorScheme *rainingColorScheme = rainingNoise->colorScheme;

    rainingColorScheme->begin = NULL;
    rainingColorScheme->end = NULL;
    rainingColorScheme->size = 0;
    rainingColorScheme->useBlock = 0;

    AddColorScheme(BLOCK_AIR, 1, 173, 0, 0, rainingColorScheme);
    AddColorScheme(BLOCK_AIR, 0.7f, 255, 70, 0, rainingColorScheme);
    AddColorScheme(BLOCK_AIR, 0.6f, 255, 120, 0, rainingColorScheme);
    AddColorScheme(BLOCK_AIR, 0.5f, 255, 182, 0, rainingColorScheme);
    AddColorScheme(BLOCK_AIR, 0.4f, 246, 250, 209, rainingColorScheme);
    AddColorScheme(BLOCK_SAND, 0.3f, 209, 229, 250, rainingColorScheme);
    AddColorScheme(BLOCK_SAND, 0.2f, 87, 120, 255, rainingColorScheme);
    AddColorScheme(BLOCK_SAND, 0.1f, 34, 0, 119, rainingColorScheme);
}

void FreeColorScheme(ColorScheme *colorScheme)
{
    if (!colorScheme)
        return;
    Scheme *scheme = colorScheme->begin;
    while (scheme != NULL)
    {
        Scheme *temp = scheme;
        scheme = temp->next;
        free(temp->color);
        free(temp);
    }
}

RGB *GetBlockColor(struct BlockInfoStruct *block, SimplexNoiseObj *noise)
{
    if (!noise || !noise->colorScheme)
    {
        printf("nose is NULL / colorScheme is NULL");
        return NULL;
    }

    Scheme *scheme = noise->colorScheme->begin;
    while (block->height < scheme->limit && scheme->next)
        scheme = scheme->next;

    if (scheme)
        return scheme->color;

    printf("scheme is NULL with height: %f \n", block->height);
    return NULL;
}

void FreeColorSchemes(SimplexNoiseObj *noise)
{
    if (!noise)
        return;
    FreeColorScheme(noise->colorScheme);
}
