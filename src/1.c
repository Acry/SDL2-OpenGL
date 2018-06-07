//BEGIN HEAD
//BEGIN DESCRIPTION
/* DEFINED PROGRESS GOALS
 * 
 * Draw a white rectangle
 * using glRectf
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
#define WW 		550
#define WH 		(WW/16)*12
#define GL_GREY 	.5, .5, .5, 1
#define GL_WHITE 	1,1,1,1
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
SDL_SetWindowTitle(Window, "Draw Rect");
SDL_ShowWindow(Window);
//END WINDOW

glClearColor(GL_GREY);
glClear(GL_COLOR_BUFFER_BIT);

glViewport (0, 0, ww, wh);

//Set Draw color to white
glColor4f(GL_WHITE);
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
	//Draw a rect
	//If you worry about the strange floats, check 2a.c
	glRectf(-.75, .75, .75, -.75);

// 	glRect(x1, y1, 	 x2, y2      ) 
//     	       Top Left, Bottom Right
//     	is exactly equivalent to the following sequence:
	
// 	glBegin(GL_QUADS);                          // Draw A Quad
// 	glVertex3f(-.75f, .75f, 0.0f);              // Top Left
// 	glVertex3f( .75f, .75f, 0.0f);              // Top Right
// 	glVertex3f( .75f,-.75f, 0.0f);              // Bottom Right
// 	glVertex3f(-.75f,-.75f, 0.0f);              // Bottom Left
// 	glEnd(); 

	/*
	 * That is what the man-page says:
	 * https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glRect.xml
	 * 
	 * BUT that isn't true!
	 * 
	 * You can't rotate a rect like that properly.
	 * And IMO the person who wrote that particular sentence into the
	 * documentation should be fired and banned from the job field.
	 * That stupid error has been there for at least ten years.
	 * 
	 * And if you check:
	 * https://www.khronos.org/members/list
	 * That is the who is who in the computing industry, even Microsoft has
	 * been there for a while before they started to provide DirectX what now
	 * bounces back.
	 * EOR - End of rant
	 * 
	 */
	
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
