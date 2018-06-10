//BEGIN HEAD
//BEGIN DESCRIPTION

/* DEFINED PROGRESS GOALS
 * Render a Texture with SDL2 as Image helper.
 * https://www.khronos.org/opengl/wiki/Image_Libraries
 */
//END   DESCRIPTION

//BEGIN INCLUDES
//system headers
#include <math.h>
//local headers
#include "helper.h"
//END   INCLUDES

//BEGIN CPP DEFINITIONS
#define GL_WHITE 	1,1,1,1
#define WW 		400
#define WH 		400
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
void assets_in	(void);
SDL_Surface * flip_vertical	(SDL_Surface *);
SDL_Surface * flip_horizontal	(SDL_Surface *);
//END	FUNCTION PROTOTYPES

//END 	HEAD

//BEGIN MAIN FUNCTION
int main(int argc, char *argv[])
{

(void)argc;
(void)argv;

//BEGIN INIT
init();
assets_in();

//BEGIN WINDOW
SDL_SetWindowPosition(Window,0,0);
SDL_SetWindowSize(Window,ww,wh);
SDL_SetWindowTitle(Window, "Draw Texture with SDL2");
SDL_ShowWindow(Window);
//END WINDOW

glClearColor(GL_WHITE);
glClear(GL_COLOR_BUFFER_BIT);
// glColor4f(1.0f, 0.5f, 1.0f, 1.0f);
glViewport (0, 0, ww, wh);
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
	glClear(GL_COLOR_BUFFER_BIT);

	glBegin(GL_QUADS);					    	// Draw textured Quad
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-.75f, .75f, 0.0f);	// Top Left Corner
	glTexCoord2f(1.0f, 0.0f); glVertex3f( .75f, .75f, 0.0f);	// Top Right Corner
	glTexCoord2f(1.0f, 1.0f); glVertex3f( .75f,-.75f, 0.0f);	// Bottom Right Corner
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-.75f,-.75f, 0.0f);	// Bottom Left Corner
	glEnd();

	SDL_GL_SwapWindow(Window);
	//END   RENDERING
}
//END   MAIN LOOP
exit_();
return EXIT_SUCCESS;

}
//END   MAIN FUNCTION

//BEGIN FUNCTIONS
void assets_in(void)
{
	//BEGIN	LOGO
	// Load surface
	SDL_Surface* surface = IMG_Load("assets/textures/tex02.jpg");
	SDL_Log("BytesPerPixel: %d",surface->format->BytesPerPixel);
// 	surface=flip_vertical(surface);
// 	surface=flip_horizontal(surface);
	glEnable(GL_TEXTURE_2D);
	
	// EnableTransparency
	// 	glEnable(GL_BLEND);
	// 	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);


// 	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	
	//Filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	
	//Wrapping
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0);

	glTexImage2D	(GL_TEXTURE_2D, 	// target
			 0, 			// level, 0 = base, no minimap,
			GL_RGB, 		// internalformat
			surface->w, 		// width
			surface->h, 		// height
			0, 			// This value must be 0.
			GL_RGB,			// format
			GL_UNSIGNED_BYTE, 	// data type of the pixel datatype
			surface->pixels);


	SDL_FreeSurface(surface);

}

SDL_Surface * flip_horizontal(SDL_Surface *sfc)
{
	// What is pitch and stride?
	// Stride means bytes in a row of pixels including any padding.
	// Pitch should be a synonym.
	
	// Pitch, absolute value, the number of bytes per bitmap line;
	// it can be either positive or negative depending on the bitmap's vertical orientation
	
	// To find a specific pixel in the padded image, you go like this:
	// Pixel = (y   * pitch) + x;
	// Pixel = (line*pitch ) +col
	
	// 	SDL_PixelFormat *format;    Read-only
	// 	int w, h;                   Read-only
	// 	int pitch;                  Read-only, the length of a row of pixels in bytes
	// 	void *pixels;               Read-write
	
	SDL_Surface *result= SDL_CreateRGBSurfaceWithFormat(0,sfc->w,sfc->h, sfc->format->BytesPerPixel, sfc->format->format);
	size_t pitch 	= sfc->pitch;
	int pxlength 	= pitch*sfc->h;
	
	void *pixels 	= sfc->pixels + pxlength;	// last pixel
	void *rpixels 	= result->pixels;		// first pixel	
	
	for(int line 	= 0; line < sfc->h; ++line){
		memcpy(rpixels,pixels,pitch);		// copy the line
		pixels -= pitch;			// count down
		rpixels += pitch;			// count up
	}
	return result;
	
}

SDL_Surface * flip_vertical(SDL_Surface *sfc)
{
	SDL_Surface *result= SDL_CreateRGBSurfaceWithFormat(0,sfc->w,sfc->h, sfc->format->BytesPerPixel, sfc->format->format);
	
	int pitch 	= sfc->pitch;
	void *pixels 	= sfc->pixels;
	void *rpixels 	= result->pixels+pitch;
	
	size_t lenght = sfc->format->BytesPerPixel;
	
	for(int line = 0; line < sfc->h; ++line){
		for (int col = 0; col < sfc->w; col++){
			memcpy(rpixels,pixels,lenght);
			pixels+=lenght;
			rpixels-=lenght;
		}
		rpixels+=2*pitch;
	}
	return result;
	
}
