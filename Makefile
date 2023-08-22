SHELL = /bin/sh
.SUFFIXES:
.SUFFIXES: .c .o

CFLAGS   = -Wall -Wextra -mtune=native `sdl2-config --cflags`
WARN_OFF = -Wno-implicit-function-declaration
CFLAGS1  = $(CFLAGS) $(WARN_OFF)
LDFLAGS  = `sdl2-config --libs` -lm -lGL
LDFLAGS1 = $(LDFLAGS)  -lSDL2_image
LDFLAGS2 = $(LDFLAGS) $(LDFLAGS1) -lSOIL
LDFLAGS3 = $(LDFLAGS1) -lGLEW
LDFLAGS4 = $(LDFLAGS)  -lGLU
LDFLAGS5 = $(LDFLAGS1) $(LDFLAGS3)
LDFLAGS6 = $(LDFLAGS5) -lSDL2_mixer

# Define the source directory
SOURCE_DIR := src/

# Define the build directory
BUILD_DIR := build/

# Create the build directory if it doesn't exist
$(shell mkdir -p $(BUILD_DIR))

TARGETS	 =  0 0a 1 1a 1b 1c 2 2a 2a1 2b 2c 2d 3 3a1 3a2 texture_SOIL texture_SDL2 texture_devil

.PHONY: all
all: $(TARGETS)

# get an OpenGL Window
0: $(SOURCE_DIR)helper.c $(SOURCE_DIR)0.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS1)

# get used OpenGL Version
0a: $(SOURCE_DIR)helper.c $(SOURCE_DIR)0a.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS1)

# draw a white rectangle
1: $(SOURCE_DIR)helper.c $(SOURCE_DIR)1.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS1)

# draw 4 rectangles
1a: $(SOURCE_DIR)helper.c $(SOURCE_DIR)1a.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS1)

# draw 4 rectangles and rotate the screen
1b: $(SOURCE_DIR)helper.c $(SOURCE_DIR)1b.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS1)

# draw 4 rectangles and rotate them individually around their center.
1c: $(SOURCE_DIR)helper.c $(SOURCE_DIR)1c.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS1)

# use shader-program, one custom vertex-shader, one custom fragment-shader
2: $(SOURCE_DIR)2.c
	$(CC) $(CFLAGS1) -o $(BUILD_DIR)$@ $+ $(LDFLAGS1)

# animate gradient using attribute and uniform
2a: $(SOURCE_DIR)2a.c
	$(CC) $(CFLAGS1) -o $(BUILD_DIR)$@ $+ $(LDFLAGS1)

# animate gradient using attribute and uniform
2a1: $(SOURCE_DIR)2a1.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS3)

# embed a default shader
2b: $(SOURCE_DIR)2b.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS3)

# embed multiple shader
2c: $(SOURCE_DIR)2c.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS3)

# make mouse work
2d: $(SOURCE_DIR)2d.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS3)

# load shadertoy fragment shader optional
3: $(SOURCE_DIR)3.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS5)

# getting Textures work
3a: $(SOURCE_DIR)3a.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS5)

# make new shader in preferred Editor
3a1: $(SOURCE_DIR)3a1.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS5)

# make new shader in preferred Editor, check file change via inotify an epoll
3a2: $(SOURCE_DIR)3a2.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS5)

#
3b: $(SOURCE_DIR)3b.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS5)

#
3c: $(SOURCE_DIR)3c.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS5)


3d: $(SOURCE_DIR)3d.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS5)

3e: $(SOURCE_DIR)3e.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS6)

texture_SOIL: $(SOURCE_DIR)helper.c $(SOURCE_DIR)texture_SOIL.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS2)

texture_SDL2: $(SOURCE_DIR)helper.c $(SOURCE_DIR)texture_SDL2.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS1)

texture_devil: $(SOURCE_DIR)helper.c $(SOURCE_DIR)texture_devil.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS1) -lIL -lILU

gradient: $(SOURCE_DIR)helper.c $(SOURCE_DIR)gradient.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS1)

4: $(SOURCE_DIR)helper.c $(SOURCE_DIR)4.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS2)

5: $(SOURCE_DIR)helper.c $(SOURCE_DIR)5.c
	$(CC) $(CFLAGS) -o $(BUILD_DIR)$@ $+ $(LDFLAGS3)

.PHONY: clean
clean:
	@rm -rf $(BUILD_DIR) 2>/dev/null || true

# Makefile Hilfe:
# $<	first dependency
# $@	name of target
# $+	list of all dependencies
# $^	list of all dependencies, no duplicates

# build after save:
# while inotifywait -e close_write ./sourcefile.c; do date +%X; make > /dev/null; done