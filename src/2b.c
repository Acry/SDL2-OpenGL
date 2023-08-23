/* Using a shader from array in header */

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> // Just for the icon - easy to strip out
#include "default_shader.h"

int ww=800;
int wh=600;

char Running = 1;
GLuint shading_program_id;

GLuint default_shader	(void);
float  fTime			(void);
void   init_glew		(void);

GLuint compile_shader		(GLenum type, GLsizei , const char **sources);
GLuint program_check		(GLuint program);

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *Window = SDL_CreateWindow("2b - Default Shader from array",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		ww, wh,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL |SDL_WINDOW_RESIZABLE);

	//BEGIN ICON
	SDL_Surface *icon;
	icon=IMG_Load("../assets/gfx/icon.png");
	SDL_SetWindowIcon(Window, icon);
	SDL_FreeSurface(icon);
	//END 	ICON

	SDL_GLContext glContext = SDL_GL_CreateContext(Window);
	init_glew();
    shading_program_id = default_shader();

	if (shading_program_id == 0){
		Running = 0;
	} else
		SDL_Log("Using program %d\n", shading_program_id);

	if (glGetError()!=0)
		SDL_Log("glError: %#08x\n", glGetError());

	glUseProgram(shading_program_id);

	static GLint uniform_gtime;
	uniform_gtime = glGetUniformLocation(shading_program_id, "fTime");

	while (Running){
		SDL_Event event;
		while ( SDL_PollEvent(&event) ){
			if (event.type == SDL_QUIT)
				Running = 0;
			else if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_ESCAPE)
				Running = 0;
			if(event.type == SDL_WINDOWEVENT){
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
					glViewport (0, 0, event.window.data1, event.window.data2);
				}
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glRectf(-1.0f, -1.0f, 1.0f, 1.0f);
		glUniform1f(uniform_gtime, fTime());
		SDL_GL_SwapWindow(Window);
	}

	SDL_GL_DeleteContext(glContext);
	SDL_Quit();
	return EXIT_SUCCESS;
}

float fTime(void)
{

	static Uint64 start 	 = 0;
	static Uint64 frequency  = 0;

	if (start==0){
		start		 =	SDL_GetPerformanceCounter();
		frequency	 =	SDL_GetPerformanceFrequency();
		return 0.0f;
	}

    Uint64 counter    	 = SDL_GetPerformanceCounter();
	Uint64 accumulate 	 = counter - start;
	return   (float)accumulate / (float)frequency;

}

void init_glew(void)
{
	GLenum status;
	status = glewInit();

	if (status != GLEW_OK){
		SDL_Log("glewInit error: %s\n", glewGetErrorString (status));
		Running = 0;
	}

	SDL_Log("\nGL_VERSION   : %s\nGL_VENDOR    : %s\nGL_RENDERER  : %s\n"
	"GLEW_VERSION : %s\nGLSL VERSION : %s\n",
	 glGetString (GL_VERSION), glGetString (GL_VENDOR),
		glGetString (GL_RENDERER), glewGetString (GLEW_VERSION),
		glGetString (GL_SHADING_LANGUAGE_VERSION));

	int maj;
	int min;
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,   &maj);
	SDL_GL_GetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,   &min);
	SDL_Log("Using OpenGL %d.%d", maj, min);

	if (!GLEW_VERSION_2_0){
		SDL_Log("At least OpenGL 2.0 with GLSL 1.10 required.");
		Running = 0;
	}
}

//BEGIN GPU PROGRAM CREATION
GLuint compile_shader(GLenum type, GLsizei sources_count, const char **sources)
{

	GLuint  shader;
	GLint   success, len;
	GLsizei i, source_len[sources_count];

	for (i = 0; i < sources_count; ++i)
        source_len[i] = (GLsizei)strlen(sources[i]);

	shader = glCreateShader(type);
	glShaderSource(shader, sources_count, sources, source_len);
	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &len);
		if (len > 1) {
			char *log;
			log = malloc(len);
			glGetShaderInfoLog(shader, len, NULL, log);
			fprintf(stderr, "%s\n\n", log);
			free(log);
		}
		SDL_Log("Error compiling shader.\n");
	}
	SDL_Log("shader: %u",shader);
	return shader;
}

GLuint program_check(GLuint program)
{
	//Error Checking
	GLint status;
	glValidateProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (!status){
		GLint len;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &len);
		SDL_Log("log length: %d",len);
		if (len > 1){
			char *log;
			log = malloc(len);
			glGetProgramInfoLog(program, sizeof(log), &len, log);
			SDL_LogError(SDL_LOG_CATEGORY_ERROR,"%s\n\n", log);
			free(log);
		}
		glDeleteProgram(program);
		SDL_Log("Error linking shader default program.\n");
		return GL_FALSE;
	}
	return GL_TRUE;
}

//END 	GPU PROGRAM CREATION

GLuint default_shader(void)
{
	GLuint vtx;
	const char *sources;
	sources = vertex_shader;
	vtx = compile_shader(GL_VERTEX_SHADER, 1, &sources);
	if (vtx==0)	return 0;

	GLuint frag;
	sources = fragment_shader;
	frag = compile_shader(GL_FRAGMENT_SHADER, 1, &sources);
	if (frag==0) return 0;

    shading_program_id = glCreateProgram();
	glAttachShader(shading_program_id, vtx);
	glAttachShader(shading_program_id, frag);
	glLinkProgram(shading_program_id);

	GLuint status;
	status=program_check(shading_program_id);
	if (status==GL_FALSE) return 0;

	return shading_program_id;
}
