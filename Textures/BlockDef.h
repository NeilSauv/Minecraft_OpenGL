#ifndef BLOCK_DEF_H
#define BLOCK_DEF_H

#define WaterLevel 0

enum BlockTypeEnum;
#include <Textures/Block.h>

typedef struct BlockInfoStruct
{
    float height;
    enum BlockTypeEnum blockType;
    struct BlockPattern *pattern;
}BlockInfoStruct;

void AddToBlockPattern(enum BlockTypeEnum block, int top, int bottom, int side1, int side2, int side3, int side4, struct BlockPattern **patterns);

#endif // !BLOCK_DEF
