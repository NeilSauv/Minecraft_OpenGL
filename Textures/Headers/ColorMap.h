#ifndef COLOR_MAP
#define COLOR_MAP

struct SimplexNoiseObj;

#include "../../Generators/Noises/Headers/NoisesHeaders.h"
#include "../../Textures/Headers/TextureHeaders.h"
#include "../../Textures/Headers/Block.h"

enum BlockTypeEnum;
struct RGB
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
};

typedef struct RGB RGB;

typedef struct RGBH
{
    unsigned int red;
    unsigned int green;
    unsigned int blue;
    float height;
}RGBH;

typedef struct BlockPattern
{
    int topFace;
    int bottomFace;
    int sideOne;
    int sideTwo;
    int sideThree;
    int sideFour;
}BlockPattern;

typedef struct Scheme
{
    float limit;
    struct RGB* color;
    enum BlockTypeEnum block;
    struct Scheme* next;
}Scheme;

struct ColorScheme
{
    int size;
    int useBlock;
    struct Scheme* begin;
    struct Scheme* end;
    struct BlockPattern** patterns;
};

typedef struct ColorScheme ColorScheme;

void AddColorScheme(enum BlockTypeEnum block, float limit, int red, int green, int blue,struct ColorScheme* colorScheme);

void InitHeightColorScheme();

void FreeColorSchemes();

RGB* GetBlockColor(struct BlockInfoStruct* block,struct SimplexNoiseObj* noise);

#endif // !COLOR_MAP
