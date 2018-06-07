#ifndef _HELPER_H_
#define _HELPER_H_

#include <stdlib.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_opengl.h> // only to the the definitions in KDev

SDL_Window   *Window;
SDL_Renderer *Renderer;

void init (void);
void exit_(void);

#endif
