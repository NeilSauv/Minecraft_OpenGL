#include "../../Textures/Headers/TextureHeaders.h"
#include "../../Generators/Chunk/Headers/ChunkHeaders.h"
#include "../../Generators/Noises/Headers/NoisesHeaders.h"

void InitBiomeAtlas()
{
	biomeAtlas = ReadBPM("ProjectFiles/Textures/Pictures/biomeAtlas.bmp");
}
