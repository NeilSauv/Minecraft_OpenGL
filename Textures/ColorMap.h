#ifndef COLOR_MAP
#define COLOR_MAP

struct SimplexNoiseObj;

#include <Textures/BitmapCreator.h>
#include <Textures/Block.h>

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

typedef struct ColorScheme
{
    int size;
    int useBlock;
    struct Scheme* begin;
    struct Scheme* end;
    struct BlockPattern** patterns;
}ColorScheme;


typedef struct ColorBiome
{
    int size;
    int useBlock;
    struct Scheme* begin;
    struct Scheme* end;
    struct BlockPattern** patterns;
}ColorBiome;

void AddColorScheme(enum BlockTypeEnum block, float limit, int red, int green, int blue,struct ColorScheme* colorScheme);

void InitHeightColorScheme();

void FreeColorSchemes(struct SimplexNoiseObj* noise);

RGB* GetBlockColor(struct BlockInfoStruct* block,struct SimplexNoiseObj* noise);

#endif // !COLOR_MAP
