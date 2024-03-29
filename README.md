# OpenGL - Playground

This is a series of OpenGL examples.

Some OpenGL 2.1 and 3.3 examples, using SDL2 and GLEW as function wrangler.
And a bit GLSL for C-Coders.

___

ARCHIVED:

This Repository is quite old and I will not update most of the code anymore.
If I find bugs I will fix them, but I will not add new features.

I corrected some misspellings und miss understandings I had back in the days and added some comments.

The 3rd part will be refactored and corrected, but then this repository will be archived.

The new work is not available publicly, since nobody really contributes to any open source code where he does not see
any benefit, beside learning.

There is a lot documentation in the code, but it is not a tutorial.
To separate code and documentation the CG-Wiki was created. And in the continued project it is used as a submodule.

___

## Legacy OpenGL

[See Chapter 3 of the graphicsbook for a good introduction to OpenGL 1.1](https://math.hws.edu/graphicsbook/c3/index.html)

 `Immediate mode` is nice because the entry barrier is extremely low.

- `0` - Init a simple OpenGL Window, on Linux it defaults to OpenGL 2.1
- `1` - Draw rectangle

![Screenshot](shots/shot_1.c.jpg)

- `1a` - Draw rectangles

![Screenshot](shots/shot_1a.c.jpg)

- `1b` - Rotate rectangles uniformly

![Screenshot](shots/shot_1b.c.jpg)

- `1c` - Rotate rectangles individually

![Screenshot](shots/shot_1c.c.jpg)

- `1d` - SGI Teapot Demo ported to SDL2 (**TBD**) - perhaps
<http://ftp.funet.fi/pub/sci/graphics/packages/objects/teasetorig.gz>

![Screenshot](shots/teapot.png)

- `1e` - SGI Atlantis Demo ported to SDL2 (**TBD**) - perhaps
<https://www.cs.kent.edu/~farrell/graphics/opengl/glut-3.0/progs/demos/atlantis/>

![Screenshot](shots/atlantis.png)

## Modern OpenGL

Retained-mode using `VAO` / `VBO`

It is much harder to do anything at all with modern OpenGL, but it is in fact easier once you are over the initial
hurdle (and, much faster).

## GLSL - Shaders

2 - Shader-Setup

![Screenshot](shots/shot_2.c.png)

![Screenshot](shots/shot_2c.c.jpg)

`2d.c` - uniform mouse and switch multiple shaders on middle mouse button

## Shadertoy - Fragment Shaders

`3` - Load a [shadertoy](https://www.shadertoy.com/) shader from command line and switch between shaders with middle mouse button.

![Screenshot](shots/shot_3.c.jpg)

Try:

- `./3 ../assets/shader/shadertoy/4ltSRS_nebula_smoke`
- `./3 ../assets/shader/shadertoy/Ms2SD1_Seascape`

`3a1` - press `n` to create new shader, will open in Editor
  if file is changed and saved. press `c` to compile.

`3a2` - should track if file in Editor was written and **auto-compile**.

- When working with shaders use left mouse button and mouse motion to change view.
- When working with multiple shaders use middle mouse button to switch shaders.

The work on Shader Toy Fragment Shaders has been split out to an extra project:
[Shade it!](https://github.com/Acry/Shade-it-)

## Image Texture Libraries

Some examples to load image textures with selected libraries:

- texture_devil
- texture_SDL2, (flipping SDL2 surfaces manually)
- texture_SOIL
- texture_stb_image

## See also

- [Training Plan](training_plan.md)
- [OpenGL - Hard Skills](hard_skills.md)

## Dependencies

`pacman -S soil devil sdl2 sdl2_image glew glu`

## Useful links

[SDL2](https://www.libsdl.org/) | [SDL-Discourse](https://discourse.libsdl.org) | [SDL2-Example Code](https://gist.github.com/Acry/baa861b8e370c6eddbb18519c487d9d8) | [N. Nopper-Code](https://github.com/McNopper/OpenGL) | [McKesson-Book](https://paroj.github.io/gltut/) | [Íñigo Quílez](http://www.iquilezles.org/prods/index.htm) | [MESA Demos](https://archive.mesa3d.org/demos/) | [BSVino/docs.gl: OpenGL Reference Documentation](https://github.com/BSVino/docs.gl) | [graphicsbook: sources for a free graphics textbook hosted at https://math.hws.edu/graphicsbook](https://github.com/davidjeck/graphicsbook/) | [opengl46-quick-reference-card.pdf](https://www.khronos.org/assets/uploads/developers/presentations/opengl46-quick-reference-card.pdf)
