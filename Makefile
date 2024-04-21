CC = gcc

INCLUDES = -I./opengl/
LIBRARIES = -L./opengl/

CFLAGS =  -Wall -Wextra -g
FREEFLAGS = -w
LDFLAGS = -lglfw -lrt -lm -ldl
TARGET = Minecraft
BUILD_DIR = _build

SRC =  main.c glad.c
SRC += Generators/Chunk/ChunkGenerator.c Generators/Chunk/ChunkManager.c Generators/Chunk/Region.c Generators/Chunk/BiomeGenerator.c
SRC += Generators/Noises/SimplexNoise.c Generators/Noises/NoiseStruct.c

SRC += Physics/Collisions.c Physics/Ray.c

SRC += Player/Camera.c Player/Controller.c Player/Destroy.c

SRC += Shaders/Shader.c

SRC += Textures/BitmapCreator.c Textures/TextureSet.c Textures/ColorMap.c Textures/DrawNoise.c Textures/Block.c

SRC += Utils/FileUtils.c Utils/List.c Utils/RayUtils.c Utils/TimeUtils.c Utils/Errors.c

OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(@D)
	$(CC) -c $(CFLAGS) $(INCLUDES) $(LIBRARIES) $< -o $@

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(INCLUDES) $(LIBRARIES) $(LDFLAGS)

all: $(TARGET)
	./$(TARGET)

free: 
	$(CC) -c $(SRC) $(INCLUDES) $(LIBRARIES) $(FREEFLAGS)
	$(CC) -o $(TARGET) $(OBJ) $(INCLUDES) $(LIBRARIES) $(LDFLAGS)
	./$(TARGET)

clean:
	-rm -f $(TARGET)
	-rm -rf $(BUILD_DIR)

.PHONY: all free clean
