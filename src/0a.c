//BEGIN HEAD
//BEGIN DESCRIPTION

/* As mentioned earlier, there is a lot of confusion around OpenGL.
 * I try to sort things out as good as I can.
 * 
 * VERSIONS
 * Let's see which Version SDL2 will provide us
 * on a Linux based OS with calling:
 * 
 * SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,   &maj);
 * SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,   &min);
 *
 * after initializing SDL.
 * 
 * SDL2 gives me OpenGL 2.1 without using a function loader.
 * That particular Version is from 2006.
 * 
 * https://www.khronos.org/opengl/wiki/History_of_OpenGL#OpenGL_2.1_.282006.29
 * 
 * Core features:
 * Addition			Promoted from
 * Pixel buffer objects		ARB_pixel_buffer_object
 * sRGB textures		EXT_texture_sRGB
 * https://www.khronos.org/registry/OpenGL/specs/gl/glspec21.pdf
 * 
 * Shading language 1.20:
 * Non-square matrices in GLSL.
 * https://www.khronos.org/registry/OpenGL/specs/gl/GLSLangSpec.1.20.pdf
 * 
 * This calls do query the Hardware.
 * const GLubyte* glGetString(GLenum name);
 * GL_VERSION
 * GL_SHADING_LANGUAGE_VERSION
 * GL_EXTENSIONS
 * 
 * HINT:
 * If you try to find a tutorial online or a book, know which version it is
 * about, beside the toolchain and language the author uses.
 * 
 * The release year of the book is a pretty good pointer which version
 * it is most likely about.
 * 
 * See:
 * https://www.khronos.org/opengl/wiki/History_of_OpenGL
 * 
 * FEATURES
 * Haven't used any so far.
 * 
 * HELPER LIBS
 * Using SDL2 at the moment.
 * 
 * https://www.khronos.org/opengl/wiki/Related_toolkits_and_APIs
 * 
 * VARIABLE TYPE NAMES
 * typedef unsigned char GLubyte; 1-byte unsigned
 * 
 */

/* DEFINED PROGRESS GOALS
 * 
 * Get provided OpenGL Version.
 * 
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

int maj;
int min;

SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,   &maj);
SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,   &min);
SDL_Log("Maj: %d Min: %d", maj, min);

const GLubyte* renderer;
renderer = glGetString (GL_RENDERER);
SDL_Log("Renderer: %s\n", renderer);

const GLubyte* version;
version = glGetString (GL_VERSION);
SDL_Log("OpenGL version supported %s\n", version);

glClearColor(GL_GREY);
glClear(GL_COLOR_BUFFER_BIT);

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
