#include <stdlib.h>

#include "../Textures/Headers/TextureHeaders.h"
#include "../Utils/Headers/UtilsHeaders.h"

void AddColorScheme(enum BlockTypeEnum block, float limit, int red, int green, int blue, ColorScheme* colorScheme)
{
	struct RGB* color = (struct RGB*)malloc(sizeof(struct RGB));

	color->red = red;
	color->green = green;
	color->blue = blue;

	Scheme* scheme = malloc(sizeof(Scheme));
	scheme->limit = limit;
	scheme->color = color;
	scheme->block = block;
	scheme->next = NULL;

	if (colorScheme->size == 0)
		colorScheme->begin = scheme;
	else
		colorScheme->end->next = scheme;

	colorScheme->end = scheme;
	colorScheme->size++;
}


void InitHeightColorScheme()
{
	heightColorScheme = malloc(sizeof(ColorScheme));
	heightColorScheme->begin = NULL;
	heightColorScheme->end = NULL;
	heightColorScheme->size = 0;
	heightColorScheme->useBlock = 1;

	AddColorScheme(Snow, 0.8f, 255, 255, 255, heightColorScheme);
	AddColorScheme(Stone, 0.7f, 163, 163, 163, heightColorScheme);
	AddColorScheme(Grass, 0.54f, 0, 255, 0, heightColorScheme);
	AddColorScheme(Sand, 0.5f, 200, 255, 0, heightColorScheme);
	AddColorScheme(Water, 0.0f, 0, 0, 255, heightColorScheme);
}

void FreeColorScheme(ColorScheme* colorScheme)
{
	Scheme* scheme = colorScheme->begin;
	Scheme* temp = malloc(sizeof(Scheme));
	while (scheme != NULL)
	{
		temp = scheme;
		scheme = temp->next;
		free(temp);
	}

	free(colorScheme);
}

void FreeColorSchemes()
{
	FreeColorScheme(heightColorScheme);
	FreeColorScheme(tempColorScheme);
}