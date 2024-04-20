CC = gcc

INCLUDES = -I./opengl/
LIBRARIES = -L./opengl/

CFLAGS =  -Wall -Wextra
FREEFLAGS = -w
LDFLAGS = -lglfw -lrt -lm -ldl
TARGET = main

SRC =  main.c glad.c
SRC += Generators/Chunk/ChunkGenerator.c Generators/Chunk/ChunkManager.c Generators/Chunk/Region.c Generators/Chunk/BiomeGenerator.c
SRC += Generators/Noises/SimplexNoise.c Generators/Noises/NoiseStruct.c

SRC += Physics/Collisions.c Physics/Ray.c

SRC += Player/Camera.c Player/Controller.c Player/Destroy.c

SRC += Shaders/Shader.c

SRC += Textures/BitmapCreator.c Textures/TextureSet.c Textures/ColorMap.c Textures/DrawNoise.c Textures/Block.c

SRC += Utils/FileUtils.c Utils/List.c Utils/RayUtils.c Utils/TimeUtils.c Utils/Errors.c

OBJ = ${SRC:.c=.o}
headers =

all:
	$(CC) -c $(SRC) $(INCLUDES) $(LIBRARIES) $(CFLAGS)
	$(CC) -o $(TARGET) *.o $(INCLUDES) $(LIBRARIES) $(LDFLAGS)
	./main

free: 
	$(CC) -c $(SRC) $(INCLUDES) $(LIBRARIES) $(FREEFLAGS)
	$(CC) -o $(TARGET) *.o $(INCLUDES) $(LIBRARIES) $(LDFLAGS)
	./main

compile: $(TARGET)
	$(CC) -o $(TARGET) *.o $(INCLUDES) $(LIBRARIES) $(LDFLAGS)

.PHONY : clean
clean :
	-rm $(TARGET) *.o
