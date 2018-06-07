//BEGIN HEAD
//BEGIN DESCRIPTION
/* DEFINED PROGRESS GOALS
 * 
 * implement range-change functions.
 * draw 4 rectangles
 * 
 */

/*
 * Coordinate Systems are a mess and most likely will always be.
 * OpenGL is right handed in object space and world space.
 * But in window space it is left handed;
 * Vulkan changes that again.
 * 
 * If you are brilliant in this section and know all axes and origins
 * in every Coordinate-Space in every API, congratlulations.
 * For people like me it is enough to be aware that an axis 
 * is not to be guaranteed to point in the direction I think or an Origin,
 * aka pivot-point (pivot french) for hinge pin, isn't at the point I am
 * thinking. I am pretty sure it was strange for many people starting SDL2
 * facing the fact the Window Coordinates Origin is top left.
 * 
 * Also, this GL works a lot with normalized values, another term which is just
 * ridiculous imo. First of all, it has nothing to do with
 * 1NF, 2NF, 3NF and BCNF in Database Normalization.
 * 
 * Normalization just means, map a value from one range to another range.
 * Changing the scale of a number.
 * 
 * E.g. RGB-Color Model values. In SDL programmers are used to 0-255 for
 * one color channel, which are 256 possible Values. Dispite the C Array-
 * Notation, where 0 is a value, it is quite intuitive; well, maybe not
 * intuitive, but for many developers a dataset range of 0-256 is very common.
 * 
 * 256×256×256 are 16,777,216 colors plus Transparency Channel.
 * 
 * In OpenGL Color-values are "normalized" or range-swapped between 0 and 1.
 * Where 0 is nothing and 1 is full.
 * All on is white and all of is black.
 * Same as for the receptors in our Eyes.
 * Must not be true for Software, hinting to X (Black-pixel and white-pixel).
 * 
 * So 256/2=128-1 would be the half of our RGB-Value for one Channel.
 * 0.5 is the normalized value.
 * 
 * Screen-Coordinates are in the range of -1 to +1, they are also called
 * Normalized Screen Coordinates (NDC).
 * In other Graphics libraries you see normalized values in the range of
 * -2 to +2, 0 to 2 or something totally different.
 * 
 * It's a matter of human arbitrariness, it is defined.
 * 
 * Hint:
 * It is way easier to search the web with terms like change range scale
 * of numbers than look for Normalization.
 * 
 * A tutorial with reasonable code is here:
 * https://github.com/Acry/C-math-normalization
 * 
 * Ofc one can implement the functions as Macros
 * in CPP DEFINITIONS
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
double norm_RGB		(double);
double rescale_RGB	(double);

double norm_NDC_x	(double);
double norm_NDC_y	(double);
double rescale_NDC_x	(double);
double rescale_NDC_y	(double);
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
SDL_SetWindowTitle(Window, "Draw Rects");
SDL_ShowWindow(Window);
//END WINDOW

glClearColor(GL_GREY);
glClear(GL_COLOR_BUFFER_BIT);

glViewport (0, 0, ww, wh);

//Set Draw color to white
glColor4f(GL_WHITE);

//I used this function call in 2.c to draw a rectangle
//	glRectf(-.75, .75, .75, -.75);

//Let's see the screen coordinates
double x1=rescale_NDC_x(-.75);
double x2=rescale_NDC_x(.75);
SDL_Log("x1: %.2f, x2: %.2f", x1, x2);

SDL_Log("ww/4: %.2f",(float)ww/4);
SDL_Log("wh/2: %.2f",(float)wh/2);

//I want a border of ten
//rect top left
double r1x1=norm_NDC_x(10);
SDL_Log("r1x1: %f", r1x1);
double r1y1=norm_NDC_y(wh-10);
SDL_Log("r1y1: %f", r1y1);
double r1x2=norm_NDC_x(ww/2-5);
SDL_Log("r1x2: %f", r1x2);
double r1y2=norm_NDC_y(wh/2+5);
SDL_Log("r1y2: %f", r1y2);

//rect bot left
double r2x1=norm_NDC_x(10);
double r2y1=norm_NDC_y(wh/2-5);
double r2x2=norm_NDC_x(ww/2-5);
double r2y2=norm_NDC_y(10);

//rect top right
double r3x1=norm_NDC_x(ww/2+5);
double r3y1=norm_NDC_y(wh-10);
double r3x2=norm_NDC_x(ww-10);
double r3y2=norm_NDC_y(wh/2+5);

//rect bot right
double r4x1=norm_NDC_x(ww/2+5);
double r4y1=norm_NDC_y(wh/2-5);
double r4x2=norm_NDC_x(ww-10);
double r4y2=norm_NDC_y(10);
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
	glRectf(r1x1, r1y1, r1x2, r1y2);
	glRectf(r2x1, r2y1, r2x2, r2y2);
	glRectf(r3x1, r3y1, r3x2, r3y2);
	glRectf(r4x1, r4y1, r4x2, r4y2);
	SDL_GL_SwapWindow(Window);
	//END   RENDERING
}
//END   MAIN LOOP


exit_();
return EXIT_SUCCESS;

}
//END   MAIN FUNCTION

//BEGIN FUNCTIONS
double norm_RGB(double x)
{
	return (x - 0) / (255 - 0);
}
double rescale_RGB(double xr)
{
	return xr * 255;
}
double norm_NDC_x(double x)
{
	return (2 * (x - 0) / (ww - 0))-1;
}
double norm_NDC_y(double y)
{
	return (2 * (y - 0) / (wh - 0))-1;
}

double rescale_NDC_x(double xr)
{
	return ww * (xr - -1) / 2;
}
double rescale_NDC_y(double xr)
{
	return wh * (xr - -1) / 2;
}
//END   FUNCTIONS
