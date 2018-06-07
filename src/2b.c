/* Using a default Shader-Program read from Array in Header */

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h> // Just for the icon - easy to strip out
#include "default_shader.h"

// #define FRAG	"assets/shader/frag/5.frag"
// #define VERT	"assets/shader/vert/2.vert"

int ww=800;
int wh=600;
char Running = 1;
GLuint shading_program;


GLuint default_shader		(void);

// loads a shader from file and returns the compiled shader
GLuint GetShader		(GLenum 	, const char *);

//Get and build custom program from 2 files
GLuint custom_shaders		(const char *	, const char *);

const char * read_file		(const char *);
float  fTime			(void);
void   init_glew		(void);

GLuint compile_shader		(GLenum type, GLsizei , const char **);
GLuint program_check		(GLuint);



int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_Window *Window = SDL_CreateWindow("2b - Default Shader",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		ww, wh,
		SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL |SDL_WINDOW_RESIZABLE);
	
	//BEGIN ICON
	SDL_Surface *icon;
	icon=IMG_Load("./assets/gfx/icon.png");
	SDL_SetWindowIcon(Window, icon);
	SDL_FreeSurface(icon);
	//END 	ICON

	SDL_GLContext glContext = SDL_GL_CreateContext(Window);
	init_glew();
// 	shading_program = custom_shaders(VERT, FRAG);
	shading_program = default_shader();
	glReleaseShaderCompiler();


	if (shading_program == 0){
		Running = 0;
	} else
		SDL_Log("Using program %d\n", shading_program);
	
	if (glGetError()!=0)
		SDL_Log("glError: %#08x\n", glGetError());
	
	glUseProgram(shading_program);
	
	static GLint uniform_gtime;
	uniform_gtime = glGetUniformLocation(shading_program, "fTime");
	
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
		glRectf(-1.0, -1.0, 1.0, 1.0);
		glUniform1f(uniform_gtime, fTime());
		SDL_GL_SwapWindow(Window);
	} 

	SDL_GL_DeleteContext(glContext);
	SDL_Quit();
	return EXIT_SUCCESS;
}

const char * read_file(const char *filename)
{
	long length = 0;
	char *result = NULL;
	FILE *file = fopen(filename, "r");
	if(file) {
		int status = fseek(file, 0, SEEK_END);
		if(status != 0) {
			fclose(file);
			return NULL;
		}
		length = ftell(file);
		status = fseek(file, 0, SEEK_SET);
		if(status != 0) {
			fclose(file);
			return NULL;
		}
		result = malloc((length+1) * sizeof(char));
		if(result) {
			size_t actual_length = fread(result, sizeof(char), length , file);
			result[actual_length++] = '\0';
		} 
		fclose(file);
		return result;
	}
	SDL_LogError(SDL_LOG_CATEGORY_ERROR,"Couldn't read %s", filename);
	return NULL;
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
	
	Uint64 counter    	 = 0;
	counter    		 = SDL_GetPerformanceCounter();
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
GLuint custom_shaders(const char *vsPath, const char *fsPath)
{
	GLuint vertexShader;
	GLuint fragmentShader;
	
	vertexShader   = GetShader(GL_VERTEX_SHADER,   vsPath);
	fragmentShader = GetShader(GL_FRAGMENT_SHADER, fsPath);
	
	shading_program = glCreateProgram();
	
	glAttachShader(shading_program, vertexShader);
	glAttachShader(shading_program, fragmentShader);
	
	glLinkProgram(shading_program);
	
	
	//Error Checking
	GLuint status;
	status=program_check(shading_program);
	if (status==GL_FALSE)
		return 0;
	return shading_program;
	
}

GLuint GetShader(GLenum eShaderType, const char *filename)
{
	
	const char *shaderSource=read_file(filename);
	GLuint shader = compile_shader(eShaderType, 1, &shaderSource);
	return shader;
	
}

GLuint compile_shader(GLenum type, GLsizei nsources, const char **sources)
{
	
	GLuint  shader;
	GLint   success, len;
	GLsizei i, srclens[nsources];
	
	for (i = 0; i < nsources; ++i)
		srclens[i] = (GLsizei)strlen(sources[i]);
	
	shader = glCreateShader(type);
	glShaderSource(shader, nsources, sources, srclens);
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
		SDL_Log("log lenght: %d",len);
		if (len > 1){
			char *log;
			log = malloc(len);
			glGetProgramInfoLog(program, sizeof(log), &len, log);
			SDL_LogError(SDL_LOG_CATEGORY_ERROR,"%s\n\n", log);
			// 			fprintf(stderr, "%s\n\n", log);
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
	if (vtx==0)
		return 0;
	
	GLuint frag;
	sources = fragment_shader;
	frag = compile_shader(GL_FRAGMENT_SHADER, 1, &sources);
	if (frag==0)
		return 0;

	shading_program = glCreateProgram();
	glAttachShader(shading_program, vtx);
	glAttachShader(shading_program, frag);
	glLinkProgram(shading_program);

	GLuint status;
	status=program_check(shading_program);
	if (status==GL_FALSE)
		return 0;
	
	return shading_program;
}
