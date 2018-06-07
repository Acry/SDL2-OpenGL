//BEGIN HEAD
//BEGIN DESCRIPTION
/* DEFINED PROGRESS GOALS
 * Draw 4 rectangles and rotate the screen
 * (viewport) with glRotate
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
SDL_SetWindowTitle(Window, "Rotate 4 Rects");
SDL_ShowWindow(Window);
//END WINDOW

glClearColor(GL_GREY);
glClear(GL_COLOR_BUFFER_BIT);

glViewport (0, 0, ww, wh);
//BEGING INIT RECTS
double rx1[4];
double rx2[4];
double ry1[4];
double ry2[4];

//rect top left
rx1[0]=norm_NDC_x(10);
ry1[0]=norm_NDC_y(wh-10);
rx2[0]=norm_NDC_x(ww/2-5);
ry2[0]=norm_NDC_y(wh/2+5);

//rect bot left
rx1[1]=norm_NDC_x(10);
ry1[1]=norm_NDC_y(wh/2-5);
rx2[1]=norm_NDC_x(ww/2-5);
ry2[1]=norm_NDC_y(10);

//rect top right
rx1[2]=norm_NDC_x(ww/2+5);
ry1[2]=norm_NDC_y(wh-10);
rx2[2]=norm_NDC_x(ww-10);
ry2[2]=norm_NDC_y(wh/2+5);

//rect bot right
rx1[3]=norm_NDC_x(ww/2+5);
ry1[3]=norm_NDC_y(wh/2-5);
rx2[3]=norm_NDC_x(ww-10);
ry2[3]=norm_NDC_y(10);
//END INIT RECTS

SDL_Event event;
int running = 1;
//END   INIT
double a=.1;
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
	glClear(GL_COLOR_BUFFER_BIT);

	glRotated(a, 0.0f, 0.0f, 1);
	// https://www.khronos.org/registry/OpenGL-Refpages/gl2.1/xhtml/glRotate.xml
	for (int i=0; i<4; i++){
		glRectd(rx1[i], ry1[i], rx2[i], ry2[i]);
	}
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
