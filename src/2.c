/* Render a colorful gradient like in:
 * 
 * https://github.com/Acry/SDL2-Surfaces/blob/master/src/7.c
 * and
 * https://github.com/Acry/SDL2-Renderer/blob/master/src/3.c
 * 
 */

/* Trying to keep it as simple as possible.
 * That is why I don't handle the arguments.
 * 
 * Vertex Shader and Fragment shader for the gradient will
 * be loaded into an array, but the path is defined.
 * 
 * See that I only included SDL.h and SDL_opengl.h.
 * I am not dealing with manual function loading
 * or using a function pointer wrangler like glew,
 * GL3W, glLoadGen, glad, GLee - whatever.
 * https://www.khronos.org/opengl/wiki/OpenGL_Loading_Library
 * 
 * So I needed -Wno-implicit-function-declaration
 * to silence gcc warnings.
 * 
 */

/*
 * Going through the process of creating and using a GPU-Program.
 * 
 * Shader loading, shader creation, program creation
 * 
 * creation means creating the object-handle
 * 
 * -get Shader Sources: File to Char Array or Char Array directly.
 * -glCreateShader type
 * -glShaderSource point compiler to source
 * -compile and return shader if everything went well
 * 
 * -glCreateProgram
 * -glAttachShader
 * -glLinkProgram
 * -glUseProgram
 * 
 * For now the code handles:
 * one custom vertex shader and one custom fragment shader.
 *
 */


#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h> // Just for the icon - easy to strip out

#define FRAG	"assets/shader/frag/2.frag"
//you can test 3.frag for a linear one colored gradient for playing around
#define VERT	"assets/shader/vert/2.vert"

/* Shader stuff:
 * 
 * Vertex attributes are used to communicate from "outside" to the vertex shader.
 * Varying variables provide an interface between Vertex and Fragment Shader.
 * 
 */
int ww=800;
int wh=600;

GLuint shading_program;

// You can either declare new OpenGL functions as extern
// or use -Wno-implicit-function-declaration to avoid warnings
extern void glGetProgramInfoLog(GLuint , GLsizei ,  GLsizei *,  GLchar *);


// loads a shader from file and returns the compiled shader
GLuint GetShader		(GLenum 	, const char *);

//Get and build custom program from 2 files
GLuint custom_shaders		(const char *	, const char *);

//see https://github.com/Acry/C-Files
const char * read_file		(const char *);

GLuint compile_shader		(GLenum type, GLsizei , const char **);
GLuint program_check		(GLuint);

int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;
	
	SDL_Init(SDL_INIT_VIDEO);
	
	SDL_Window *Window = SDL_CreateWindow("0 - Color Gradient",
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
	
	shading_program = custom_shaders(VERT, FRAG);
	glReleaseShaderCompiler();

	char Running = 1;
	if (shading_program == 0){
		Running = 0;
	} else
		SDL_Log("Using program %d\n", shading_program);
	
	if (glGetError()!=0)
		SDL_Log("glError: %#08x\n", glGetError());
	
	glUseProgram(shading_program);

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
		if (len > 1){
			char *log;
			log = malloc(len);
			glGetProgramInfoLog(program, len, &len, log);
			fprintf(stderr, "%s\n\n", log);
			free(log);
		}
		SDL_Log("Error linking shader default program.\n");
		return GL_FALSE;
	}
	return GL_TRUE;
}
//END 	GPU PROGRAM CREATION
