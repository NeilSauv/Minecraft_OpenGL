#ifndef BLOCK
#define BLOCK

#define WaterLevel 64

typedef enum BlockTypeEnum
{
	Air,
	Grass,
	Dirt,
	Snow,
	Stone,
	Water,
	Sand
}BlockTypeEnum;

struct BlockInfoStruct
{
	float height;
	enum BlockTypeEnum blockType;
	struct BlockPattern* pattern;
};

typedef struct BlockInfoStruct BlockInfoStruct;

#endif // !BLOCK