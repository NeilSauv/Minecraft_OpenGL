#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <malloc.h>

#include "../Textures/Headers/TextureHeaders.h"
#include "../Utils/Headers/UtilsHeaders.h"
#include "../Generators/Noises/Headers/NoisesHeaders.h"

#define _height ChunkSize*ChunkView*2
#define _width  ChunkSize*ChunkView*2
#define _bitsperpixel 24
#define _planes 1
#define _compression 0
#define _pixelbytesize _height*_width*_bitsperpixel/8
#define _filesize _pixelbytesize+sizeof(bitmap)
#define _xpixelpermeter 0x130B //2835 , 72 DPI
#define _ypixelpermeter 0x130B //2835 , 72 DPI
#define pixel 0xFF
#pragma pack(push,1)

void ColorBMP(SimplexNoiseObj* noise);
void MonoBMP(SimplexNoiseObj* noise);

typedef struct {
    char signature[2];
    uint32_t filesize;
    uint32_t reserved;
    uint32_t fileoffset_to_pixelarray;
} fileheader;

typedef struct {
    uint32_t dibheadersize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitsperpixel;
    uint32_t compression;
    uint32_t imagesize;
    uint32_t ypixelpermeter;
    uint32_t xpixelpermeter;
    uint32_t numcolorspallette;
    uint32_t mostimpcolor;
} bitmapinfoheader;
typedef struct {
    fileheader fileheader;
    bitmapinfoheader bitmapinfoheader;
} bitmap;
#pragma pack(pop)

void CreateBMP(SimplexNoiseObj* noise) 
{
    MonoBMP(noise);
    ColorBMP(noise);
}

void ColorBMP(SimplexNoiseObj* noise)
{
    FILE* fp = fopen("MapColor.bmp", "wb");

    bitmap* pbitmap = calloc(1, sizeof(bitmap));

    unsigned char* pixels = calloc(1, _height * _width * 3* sizeof(unsigned char));

    strcpy(pbitmap->fileheader.signature, "BM");
    pbitmap->fileheader.filesize = _filesize;
    pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap);
    pbitmap->bitmapinfoheader.dibheadersize = sizeof(bitmapinfoheader);
    pbitmap->bitmapinfoheader.width = _width;
    pbitmap->bitmapinfoheader.height = _height;
    pbitmap->bitmapinfoheader.planes = _planes;
    pbitmap->bitmapinfoheader.bitsperpixel = _bitsperpixel;
    pbitmap->bitmapinfoheader.compression = _compression;
    pbitmap->bitmapinfoheader.imagesize = _pixelbytesize;
    pbitmap->bitmapinfoheader.ypixelpermeter = _ypixelpermeter;
    pbitmap->bitmapinfoheader.xpixelpermeter = _xpixelpermeter;
    pbitmap->bitmapinfoheader.numcolorspallette = 0;

    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            int p = (y * _height + x) * 3;
            pixels[p + 0] = noise->noiseMap[x][y]->blue;//blue
            pixels[p + 1] = noise->noiseMap[x][y]->green;//green
            pixels[p + 2] = noise->noiseMap[x][y]->red;//red   
        }
    }

    fwrite(pbitmap, 1, sizeof(bitmap), fp);
    fwrite(pixels, 1, _pixelbytesize, fp);
    fclose(fp);
    free(pbitmap);
    free(pixels);
}

void MonoBMP(SimplexNoiseObj* noise)
{
    FILE* fp = fopen("MapBW.bmp", "wb");

    bitmap* pbitmap = calloc(1, sizeof(bitmap));
    unsigned char* pixels = malloc(_height * _width * 3);

    strcpy(pbitmap->fileheader.signature, "BM");
    pbitmap->fileheader.filesize = _filesize;
    pbitmap->fileheader.fileoffset_to_pixelarray = sizeof(bitmap);
    pbitmap->bitmapinfoheader.dibheadersize = sizeof(bitmapinfoheader);
    pbitmap->bitmapinfoheader.width = _width;
    pbitmap->bitmapinfoheader.height = _height;
    pbitmap->bitmapinfoheader.planes = _planes;
    pbitmap->bitmapinfoheader.bitsperpixel = _bitsperpixel;
    pbitmap->bitmapinfoheader.compression = _compression;
    pbitmap->bitmapinfoheader.imagesize = _pixelbytesize;
    pbitmap->bitmapinfoheader.ypixelpermeter = _ypixelpermeter;
    pbitmap->bitmapinfoheader.xpixelpermeter = _xpixelpermeter;
    pbitmap->bitmapinfoheader.numcolorspallette = 0;

    for (int y = 0; y < _height; y++) {
        for (int x = 0; x < _width; x++) {
            int p = (y * _height + x) * 3;
            int val = (noise->blocks[x][y]->height + 1) / 2 * 255;
            pixels[p + 0] = val;//blue
            pixels[p + 1] = val;//green
            pixels[p + 2] = val;//red   
        }
    }

    fwrite(pbitmap, 1, sizeof(bitmap), fp);
    fwrite(pixels, 1, _pixelbytesize, fp);
    fclose(fp);
    free(pbitmap);
    free(pixels);
}