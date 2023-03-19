CC = gcc

INCLUDES = -I/mnt/c/Users/neils/Documents/opengl/
LIBRARIES = -L/mnt/c/Users/neils/Documents/opengl/

CFLAGS = -Wall -Werror
FREEFLAGS = -w
LDFLAGS = -lglfw -lrt -lm -ldl -O3
TARGET = main

SRC =  main.c glad.c
SRC += Generators/Chunk/ChunkGenerator.c Generators/Chunk/ChunkManager.c Generators/Chunk/Region.c
SRC += Generators/Noises/SimplexNoise.c

SRC += Physics/Collisions.c Physics/Ray.c

SRC += Player/Camera.c Player/Controller.c Player/Destroy.c

SRC += Shaders/Shader.c

SRC += Textures/BitmapCreator.c Textures/TextureSet.c Textures/ColorMap.c Textures/DrawNoise.c

SRC += Utils/FileUtils.c Utils/List.c Utils/RayUtils.c Utils/TimeUtils.c

SRC += Externals/nanovg/android.c Externals/nanovg/blendish.c Externals/nanovg/nanosvg.c Externals/nanovg/nanosvgrast.c Externals/nanovg/nanovg.c Externals/nanovg/nanovg_gl.c Externals/nanovg/nanovg_gl_utils.c


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
