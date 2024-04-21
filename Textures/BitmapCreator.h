#ifndef BITMAP_CREATOR
#define BITMAP_CREATOR

#include <Generators/Chunk/ChunkGenerator.h>

#define _height ChunkSize *ChunkView * 2
#define _width ChunkSize *ChunkView * 2
#define _bitsperpixel 24
#define _planes 1
#define _compression 0
#define _pixelbytesize _height *_width *_bitsperpixel / 8
#define _filesize _pixelbytesize + sizeof(bitmap)
#define _xpixelpermeter 0x130B // 2835 , 72 DPI
#define _ypixelpermeter 0x130B // 2835 , 72 DPI
#define pixel 0xFF
// #pragma pack(push,1)

struct SimplexNoiseObj;
void CreateBMP(struct SimplexNoiseObj *noise, char *name);

void BiomeBPM();

#endif
