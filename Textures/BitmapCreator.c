#include "BitmapCreator.h"

#include <Generators/Chunk/BiomeGenerator.h>
#include <Generators/Noises/SimplexNoise.h>
#include <malloc.h>
#include <stdio.h>
#include <string.h>

#include "bits/stdint-uintn.h"
#pragma pack(push, 1)
void ColorBMP(SimplexNoiseObj *noise, char *name);
void MonoBMP(SimplexNoiseObj *noise, char *name);

typedef struct
{
    char signature[2];
    uint32_t filesize;
    uint32_t reserved;
    uint32_t fileoffset_to_pixelarray;
} fileheader;

typedef struct
{
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
typedef struct
{
    fileheader fileheader;
    bitmapinfoheader bitmapinfoheader;
} bitmap;
#pragma pack(pop)
char *Concatenate(char *folder, char *fileName, char *extension)
{
    int lenFolder = 0;
    int lenFileName = 0;
    int lenExtension = 0;

    for (int i = 0; folder[i] != 0; i++)
        lenFolder++;

    for (int i = 0; fileName[i] != 0; i++)
        lenFileName++;

    for (int i = 0; extension[i] != 0; i++)
        lenExtension++;

    int total = lenFolder + lenFileName + lenExtension;

    char *path = malloc(sizeof(char) * (total + 1));

    for (int i = 0; i < lenFolder; i++)
        path[i] = folder[i];

    for (int i = 0; i < lenFileName; i++)
        path[i + lenFolder] = fileName[i];

    for (int i = 0; i < lenExtension; i++)
        path[i + lenFolder + lenFileName] = extension[i];

    path[total] = 0;

    return path;
}

void CreateBMP(SimplexNoiseObj *noise, char *name)
{
    MonoBMP(noise, name);
    ColorBMP(noise, name);
}

void ColorBMP(SimplexNoiseObj *noise, char *name)
{
    char *path = Concatenate("test/", name, "_RGB.bmp");
    FILE *fp = fopen(path, "wb");

    bitmap *pbitmap = calloc(1, sizeof(bitmap));

    unsigned char *pixels =
        calloc(1, _height * _width * 3 * sizeof(unsigned char));

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

    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            int p = (y * _height + x) * 3;
            pixels[p + 0] = noise->noiseMap[y][x]->blue; // blue
            pixels[p + 1] = noise->noiseMap[y][x]->green; // green
            pixels[p + 2] = noise->noiseMap[y][x]->red; // red
        }
    }

    fwrite(pbitmap, 1, sizeof(bitmap), fp);
    fwrite(pixels, 1, _pixelbytesize, fp);
    fclose(fp);
    free(pbitmap);
    free(pixels);
}

void MonoBMP(struct SimplexNoiseObj *noise, char *name)
{
    char *path = Concatenate("test/", name, "_BW.bmp");
    FILE *fp = fopen(path, "wb");

    bitmap *pbitmap = calloc(1, sizeof(bitmap));
    unsigned char *pixels = malloc(_height * _width * 3);

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

    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            int p = (y * _height + x) * 3;
            int val = (float)(noise->noiseMap[y][x]->height + 1) / 2.f * 255.f;
            pixels[p + 0] = val; // blue
            pixels[p + 1] = val; // green
            pixels[p + 2] = val; // red
        }
    }

    fwrite(pbitmap, 1, sizeof(bitmap), fp);
    fwrite(pixels, 1, _pixelbytesize, fp);
    fclose(fp);
    free(pbitmap);
    free(pixels);
}

void BiomeBPM()
{
    char *path = "test/Biomes_RGB.bmp";

    FILE *fp = fopen(path, "wb");

    bitmap *pbitmap = calloc(1, sizeof(bitmap));
    unsigned char *pixels = malloc(_height * _width * 3);

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

    for (int y = 0; y < _height; y++)
    {
        for (int x = 0; x < _width; x++)
        {
            int p = (y * _height + x) * 3;

            int temperature =
                (float)(temperatureNoise->noiseMap[y][x]->height + 1) / 2 * 250;
            int raining =
                (float)(rainingNoise->noiseMap[y][x]->height + 1) / 2 * 250;

            pixels[p + 0] =
                biomeAtlas[temperature * 3 * 250 + raining * 3 + 2]; // blue
            pixels[p + 1] =
                biomeAtlas[temperature * 3 * 250 + raining * 3 + 1]; // green
            pixels[p + 2] =
                biomeAtlas[temperature * 3 * 250 + raining * 3]; // red
        }
    }

    fwrite(pbitmap, 1, sizeof(bitmap), fp);
    fwrite(pixels, 1, _pixelbytesize, fp);
    fclose(fp);
    free(pbitmap);
    free(pixels);
}
