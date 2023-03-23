#include <stdlib.h>

#include "../Generators/Noises/Headers/NoisesHeaders.h"
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
	ColorScheme* heightColorScheme = heightNoise->colorScheme;

	heightColorScheme->begin = NULL;
	heightColorScheme->end = NULL;
	heightColorScheme->size = 0;
	heightColorScheme->useBlock = 1;

	AddColorScheme(Snow, 0.8f, 255, 255, 255, heightColorScheme);
	AddColorScheme(Stone, 0.7f, 163, 163, 163, heightColorScheme);
	AddColorScheme(Grass, 0.54f, 0, 255, 0, heightColorScheme);
	AddColorScheme(Sand, 0.5f, 200, 255, 0, heightColorScheme);
	AddColorScheme(Water, 0.0f, 0, 0, 255, heightColorScheme);
	AddColorScheme(Dirt, 0.0f, 0, 0, 255, heightColorScheme);


	ColorScheme* temperatureColorScheme = temperatureNoise->colorScheme;

	temperatureColorScheme->begin = NULL;
	temperatureColorScheme->end = NULL;
	temperatureColorScheme->size = 0;
	temperatureColorScheme->useBlock = 0;

	AddColorScheme(Air, 0.8f, 173, 0,0, temperatureColorScheme);
	AddColorScheme(Air, 0.7f, 255, 70, 0, temperatureColorScheme);
	AddColorScheme(Air, 0.6f, 255, 120, 0, temperatureColorScheme);
	AddColorScheme(Air, 0.5f, 255, 182, 0, temperatureColorScheme);
	AddColorScheme(Air, 0.4f, 246, 250, 209, temperatureColorScheme);
	AddColorScheme(Air, 0.3f, 209, 229, 250, temperatureColorScheme);
	AddColorScheme(Air, 0.2f, 87, 120, 255, temperatureColorScheme);
	AddColorScheme(Air, 0.1f, 34, 0, 119, temperatureColorScheme);


	ColorScheme* rainingColorScheme = rainingNoise->colorScheme;

	rainingColorScheme->begin = NULL;
	rainingColorScheme->end = NULL;
	rainingColorScheme->size = 0;
	rainingColorScheme->useBlock = 0;

	AddColorScheme(Air, 0.8f, 173, 0, 0, rainingColorScheme);
	AddColorScheme(Air, 0.7f, 255, 70, 0, rainingColorScheme);
	AddColorScheme(Air, 0.6f, 255, 120, 0, rainingColorScheme);
	AddColorScheme(Air, 0.5f, 255, 182, 0, rainingColorScheme);
	AddColorScheme(Air, 0.4f, 246, 250, 209, rainingColorScheme);
	AddColorScheme(Air, 0.3f, 209, 229, 250, rainingColorScheme);
	AddColorScheme(Air, 0.2f, 87, 120, 255, rainingColorScheme);
	AddColorScheme(Air, 0.1f, 34, 0, 119, rainingColorScheme);

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

RGB* GetBlockColor(BlockInfoStruct* block, SimplexNoiseObj* noise)
{
	ColorScheme* colorScheme = noise->colorScheme;

	Scheme* scheme = colorScheme->begin;
	while (block->height < scheme->limit)
		scheme = scheme->next;

	return scheme->color;
}

void FreeColorSchemes()
{
	FreeColorScheme(heightNoise->colorScheme);
	FreeColorScheme(heightNoise->colorScheme);
}