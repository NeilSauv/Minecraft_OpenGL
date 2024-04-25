#include "BlockDef.h"

#include <stdlib.h>

#include "Generators/Noises/NoiseStruct.h"
#include "Textures/ColorMap.h"

void AddToBlockPattern(enum BlockTypeEnum block, int top, int bottom, int side1,
                       int side2, int side3, int side4,
                       struct BlockPattern **patterns)
{
    struct BlockPattern *pattern = malloc(sizeof(BlockPattern));
    pattern->topFace = top;
    pattern->bottomFace = bottom;
    pattern->sideOne = side1;
    pattern->sideTwo = side2;
    pattern->sideThree = side3;
    pattern->sideFour = side4;

    patterns[block] = pattern;

    blockPatterns[block * 6 + 0] = top;
    blockPatterns[block * 6 + 1] = side1;
    blockPatterns[block * 6 + 2] = side2;
    blockPatterns[block * 6 + 3] = side3;
    blockPatterns[block * 6 + 4] = side4;
    blockPatterns[block * 6 + 5] = bottom;
}
