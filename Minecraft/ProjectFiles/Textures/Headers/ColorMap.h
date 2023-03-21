#ifndef COLOR_MAP
#define COLOR_MAP

#include "../../Textures/Headers/TextureHeaders.h"

struct RGB
{
	unsigned int red;
	unsigned int green;
	unsigned int blue;
};

typedef struct RGB RGB;

struct Scheme
{
	float limit;
	struct RGB* color;
	enum BlockTypeEnum block;
	struct Scheme* next;
};

typedef struct Scheme Scheme;

struct ColorScheme
{
	int size;
	int useBlock;
	struct Scheme* begin;
	struct Scheme* end;
};

typedef struct ColorScheme ColorScheme;

struct ColorScheme* heightColorScheme;

struct ColorScheme* tempColorScheme;

void AddColorScheme(enum BlockTypeEnum block, float limit, int red, int green, int blue,struct ColorScheme* colorScheme);

void InitHeightColorScheme();

void FreeColorSchemes();

#endif // !COLOR_MAP
