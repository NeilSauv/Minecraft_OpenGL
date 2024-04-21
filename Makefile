CC = gcc

INCLUDES = -I./opengl -I./
LIBRARIES = -L./opengl/

CFLAGS =  -Wall -Wextra -g -Werror -std=c99
FREEFLAGS = -w
LDFLAGS = -lglfw -lrt -lm -ldl
TARGET = Minecraft
BUILD_DIR = _build

SRC = $(shell find . -type f -name '*.c')
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
