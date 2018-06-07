CFLAGS   = -Wall -Wextra -mtune=native `sdl2-config --cflags`
WARN_OFF = -Wno-implicit-function-declaration
CFLAGS1  = $(CFLAGS) $(WARN_OFF)
LDFLAGS  = `sdl2-config --libs` -lm -lGL
LDFLAGS1 = $(LDFLAGS)  -lSDL2_image

LDFLAGS2 = $(LDFLAGS)  -lSOIL
LDFLAGS3 = $(LDFLAGS1) -lGLEW
LDFLAGS4 = $(LDFLAGS)  -lGLU
LDFLAGS5 = $(LDFLAGS1) $(LDFLAGS3)
LDFLAGS6 = $(LDFLAGS5) -lSDL2_mixer

.SUFFIXES:
.SUFFIXES: .c .o

srcdir	 =src/
TARGETS	 =  0 0a 1 1a 1b 1c 2 2a 2a1 2b 2c 2d 3

#for future code
#texture 3a 3b 3c 3d 3e

.PHONY: all
all: $(TARGETS)

# get an OpenGL Window
0: $(srcdir)helper.c $(srcdir)0.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS1)

# get used OpenGL Version
0a: $(srcdir)helper.c $(srcdir)0a.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS1)

# draw a white rectangle
1: $(srcdir)helper.c $(srcdir)1.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS1)

# draw 4 rectangles
1a: $(srcdir)helper.c $(srcdir)1a.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS1)

# draw 4 rectangles and rotate the screen
1b: $(srcdir)helper.c $(srcdir)1b.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS1)

# draw 4 rectangles and rotate them individually around their center.
1c: $(srcdir)helper.c $(srcdir)1c.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS1)

# use shader-program, one custom vertex-shader, one custom fragment-shader
2: $(srcdir)2.c
	$(CC) $(CFLAGS1) -o $@ $+ $(LDFLAGS1)

# animate gradient using attribute and uniform
2a: $(srcdir)2a.c
	$(CC) $(CFLAGS1) -o $@ $+ $(LDFLAGS1)

# animate gradient using attribute and uniform
2a1: $(srcdir)2a1.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS3)

# embed a default shader
2b: $(srcdir)2b.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS3)

# embed a multiple shader
2c: $(srcdir)2c.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS3)

# make mouse work
2d: $(srcdir)2d.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS3)

# load shadertoy fragment shader optional
3: $(srcdir)3.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS5)

# embed a default shader
3a: $(srcdir)3a.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS5)

# embed multiple default shaders and switch program during runtime
3b: $(srcdir)3b.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS5)

# load shadertoy fragment shader optional
3c: $(srcdir)3c.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS5)


3d: $(srcdir)3d.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS5)

3e: $(srcdir)3e.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS6)

texture: $(srcdir)helper.c $(srcdir)texture.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS2)

4: $(srcdir)helper.c $(srcdir)4.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS2)

5: $(srcdir)helper.c $(srcdir)5.c
	$(CC) $(CFLAGS) -o $@ $+ $(LDFLAGS3)

.PHONY: clean
clean:
	@rm $(TARGETS) 2>/dev/null || true

