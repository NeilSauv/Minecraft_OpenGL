#ifndef BLOCK
#define BLOCK

#define WaterLevel 64

enum BlockTypeEnum
{
	Air = 0,
	Grass = 1,
	Snow = 2,
	Stone = 3,
	Water = 4,
	Sand = 5
};

typedef enum BlockTypeEnum BlockTypeEnum;

struct BlockInfoStruct
{
	int height;
	enum BlockTypeEnum blockType;
};

typedef struct BlockInfoStruct BlockInfoStruct;

#endif // !BLOCK