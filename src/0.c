//BEGIN HEAD
//BEGIN DESCRIPTION

/* Init a simple OpenGL Window */

/* This is a starter series about GLSL Shaders using SDL2 as OpenGL Helper and
 * a bit Shader-Language for C-Coders.
 * 
 * Also the GL-API defines quite a sophisticated working set, which is pretty
 * mature but also very cluttered when it comes to Extensions.
 * 
 * It's quite normal, the same happened to the X-Window System.
 * You can't have flexibility without complexity.
 * 
 * And those API's are around for a long time in terms of
 * software-age and both are supporting a vast amount of hardware
 * and have tons of features.
 * 
 * That and the fact that many developers were working on DirectX
 * only and now coming back to OGL since Android and IOS don't support
 * DX, lead to the development of Vulkan, which is the successor to OpenGL
 * like Wayland is to the X-Display-System.
 * 
 * However, learning GLSL does open the door to modern Graphics Hardware,
 * the GPU offers thousands of parallel computing cores and GLSL can be used
 * with Vulkan also.
 */

/* DEFINED PROGRESS GOALS
 * 
 * Initialize an OpenGL-Window
 * 
 * #include <SDL2/SDL_opengl.h> in helper.h
 * so I need also -lGL in the LDFLAGS
 * 
 * In helper.c
 * Init:
 * add SDL_WINDOW_OPENGL Flag to the window creation call.
 * SDL_GLContext glcontext = SDL_GL_CreateContext(Window);
 * SDL_GLContext is an alias for void *, a void pointer.
 * 
 * Exit:
 * SDL_GL_DeleteContext(glcontext);
 * 
 * SDL_GL_SwapWindow(Window) replaces SDL_RenderPresent(Renderer);
 * Not going to deal with the mix of both API's here!
 */

//END   DESCRIPTION

//BEGIN INCLUDES

//system headers
#include <math.h>
//local headers
#include "helper.h"

//END   INCLUDES

//BEGIN CPP DEFINITIONS

#define GL_GREY .5, .5, .5, 1

#define WW 	550
#define WH 	(WW/16)*12

//END   CPP DEFINITIONS

//BEGIN DATASTRUCTURES

//END	DATASTRUCTURES

//BEGIN GLOBALS

int ww=WW;
int wh=WH;

//BEGIN VISIBLES

//END 	VISIBLES

//END   GLOBALS

//BEGIN FUNCTION PROTOTYPES

//END	FUNCTION PROTOTYPES

//END 	HEAD

//BEGIN MAIN FUNCTION

int main(int argc, char *argv[])
{

(void)argc;
(void)argv;

//BEGIN INIT

init();

//BEGIN WINDOW

SDL_SetWindowPosition(Window,0,0);
SDL_SetWindowSize(Window,ww,wh);
SDL_SetWindowTitle(Window, "OpenGL Window");
SDL_ShowWindow(Window);

//END WINDOW

// Don't clear a window to black, it's harder to debug!
glClearColor(GL_GREY);
// This will give a grey
// it is a call like SDL_SetRenderDrawColor(Renderer,0,0,0,0);
// but with range swapped values 127 in this case

glClear(GL_COLOR_BUFFER_BIT);
// similar to SDL_RenderClear(Renderer)
// void glClear(GLbitfield mask);

/*
 * The values are as follows:
 * 
 * GL_COLOR_BUFFER_BIT
 * Indicates the buffers currently enabled for color writing.
 * 
 * GL_DEPTH_BUFFER_BIT
 * Indicates the depth buffer.
 * 
 * GL_ACCUM_BUFFER_BIT
 * Indicates the accumulation buffer.
 * 
 * GL_STENCIL_BUFFER_BIT
 * Indicates the stencil buffer.
 * 
 * The value to which each buffer is cleared depends on the setting of the
 * clear value for that buffer.
 * 
 */

SDL_Event event;
int running = 1;

//END   INIT

//BEGIN MAIN LOOP

while(running){

	//BEGIN EVENT LOOP

	while(SDL_PollEvent(&event)){
		if(event.type == SDL_QUIT){
			running =0;
		}
		if(event.type == SDL_MOUSEMOTION){
			;
		}
		if(event.type == SDL_MOUSEBUTTONDOWN){
			if(event.button.button == SDL_BUTTON_RIGHT){
				;
			}
			if(event.button.button == SDL_BUTTON_MIDDLE){
				;
			}
			if(event.button.button==SDL_BUTTON_LEFT){
				;
			}
		}
		if(event.type == SDL_KEYDOWN ){
			switch(event.key.keysym.sym ){
				case SDLK_ESCAPE:
					running =0;
					break;

				case SDLK_r:
				case SDLK_BACKSPACE:
					break;

				case SDLK_p:	
				case SDLK_SPACE:
					break;
					
				default:
					break;
			}
		}
	}

	//END   EVENT LOOP

	//BEGIN RENDERING

	SDL_GL_SwapWindow(Window);

	//END   RENDERING

}

//END   MAIN LOOP

exit_();
return EXIT_SUCCESS;

}
//END   MAIN FUNCTION

//BEGIN FUNCTIONS

//END   FUNCTIONS
