#include "helper.h"
SDL_Window   	*Window	  = NULL;
SDL_Renderer 	*Renderer = NULL;
SDL_GLContext 	glcontext = NULL;

void init(void)
{

SDL_Init(SDL_INIT_EVERYTHING);
Window = SDL_CreateWindow("", 0, 0, 0, 0, SDL_WINDOW_HIDDEN|SDL_WINDOW_OPENGL);
glcontext = SDL_GL_CreateContext(Window);
//BEGIN ICON
SDL_Surface *icon;
icon=IMG_Load("./assets/gfx/icon.png");
SDL_SetWindowIcon(Window, icon);
SDL_FreeSurface(icon);
//END 	ICON

}

void exit_(void)
{

SDL_GL_DeleteContext(glcontext); 
SDL_DestroyWindow(Window);
SDL_Quit();

}
