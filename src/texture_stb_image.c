//BEGIN HEAD
//BEGIN DESCRIPTION

/* DEFINED PROGRESS GOALS
 * Render a Texture with stb_image as Image helper.
 * https://github.com/nothings/stb/blob/master/stb_image.h
 * https://www.khronos.org/opengl/wiki/Image_Libraries
 */
//END   DESCRIPTION

//BEGIN INCLUDES
//system headers

//local headers
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
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
GLuint textureId;
//END 	VISIBLES

//END   GLOBALS

//BEGIN FUNCTION PROTOTYPES
void assets_in	(void);
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
SDL_SetWindowTitle(Window, "Draw Texture with stb_image");
SDL_ShowWindow(Window);
//END WINDOW

glClearColor(GL_WHITE);
glClear(GL_COLOR_BUFFER_BIT);

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
glDeleteTextures( 1, &textureId );
exit_();
return EXIT_SUCCESS;

}
//END   MAIN FUNCTION

//BEGIN FUNCTIONS
void assets_in(void)
{
    int imageWidth, imageHeight, imageChannels;
    unsigned char* image = stbi_load("../assets/textures/tex02.jpg", &imageWidth, &imageHeight, &imageChannels, STBI_rgb_alpha);
    if (image)
    {
        printf("loaded texture image.\n");
    }
    else
    {
        printf("Failed to load texture image.\n");
    }

    SDL_Log("imageWidth: %d", imageWidth);
    SDL_Log("imageHeight: %d", imageHeight);

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            imageWidth,
            imageHeight,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            image
    );

    // Set texture parameters (you can customize these)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_image_free(image);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_TEXTURE_2D);
}
//END   FUNCTIONS
