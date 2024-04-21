CC = gcc

INCLUDES = -I./opengl -I./
LIBRARIES = -L./opengl/

CFLAGS =  -Wall -Wextra -g -Werror -std=c99
FREEFLAGS = -w
LDFLAGS = -lglfw -lrt -lm -ldl
TARGET = Minecraft
BUILD_DIR = _build
IMAGE_DIR = test

SRC = $(shell find . -type f -name '*.c')
HEADERS = $(shell find . -type f -name '*.h')
OBJ = $(SRC:%.c=$(BUILD_DIR)/%.o)

all: format $(TARGET)

$(BUILD_DIR)/%.o: %.c $(HEADERS)
	@mkdir -p $(@D)
	@mkdir -p $(IMAGE_DIR)
	$(CC) -c $(CFLAGS) $(INCLUDES) $(LIBRARIES) $< -o $@

$(TARGET): $(OBJ)
	$(CC) -o $(TARGET) $(OBJ) $(INCLUDES) $(LIBRARIES) $(LDFLAGS)

format:
	find . -path './opengl' -prune -o -iname '*.h' -o -iname '*.c' -print | xargs clang-format -i

free: 
	$(CC) -c $(SRC) $(INCLUDES) $(LIBRARIES) $(FREEFLAGS)
	$(CC) -o $(TARGET) $(OBJ) $(INCLUDES) $(LIBRARIES) $(LDFLAGS)
	./$(TARGET)

clean:
	-rm -f $(TARGET)
	-rm -rf $(BUILD_DIR)

.PHONY: all free clean format
