/*
 * 2a.c - Animated Gradient
 * This program does NOT work as expected.
 * It is here for reference.
 * I have no clue why it fails.
 */

/* Render a colorful gradient like in:
 *
 * https://github.com/Acry/SDL2-Surfaces/blob/master/src/7.c
 * and
 * https://github.com/Acry/SDL2-Renderer/blob/master/src/3.c
 *
 * and animate it.
 */

/* We need a timing function to provide time passed in float.
 * A uniform in the fragment shader:
 * uniform 	float fTime;
 * And use it int the equation to define the fragment color.
 */

/*
 * BUT - All I could get was this white flickering.
 * This was really bugging me, if you can explain to me why this doesn't
 * work, please contact me. IMO it should, but I needed to use glew to get it
 * running like expected.
 * look 2a1 for the expected output.
 * - fixed shader version mess, no impact
 */

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h> // Just for the icon - easy to strip out

#define FRAG	"../assets/shader/frag/5.frag"
#define VERT	"../assets/shader/vert/2.vert"
/*
 * Uniform variables are used to communicate with your vertex or fragment shader
 * from "outside". In your shader you use the uniform qualifier to declare the
 * variable.
 *
 * As of GLSL 130+, in and out are used instead of attribute and varying.
 *
 * I like this overview:
 * https://github.com/mattdesl/lwjgl-basics/wiki/glsl-versions
 */

int ww=800;
int wh=600;
char Running = 1;
GLuint shading_program_id;

// You can either declare new OpenGL functions as extern
// or use -Wno-implicit-function-declaration to avoid warnings
// extern void glGetProgramInfoLog(GLuint , GLsizei ,  GLsizei *,  GLchar *);


// loads a shader from file and returns the compiled shader
GLuint GetShader		(GLenum 	, const char *);

//Get and build custom program from 2 files
GLuint custom_shaders		(const char *	, const char *);

const char * read_file		(const char *);
float  fTime			(void);

GLuint compile_shader		(GLenum type, GLsizei , const char **);
GLuint program_check		(GLuint);



int main(int argc, char *argv[])
{
	(void)argc;
	(void)argv;

	SDL_Init(SDL_INIT_VIDEO);

	SDL_Window *Window = SDL_CreateWindow("2a - Animated Gradient",
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

    shading_program_id = custom_shaders(VERT, FRAG);

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


//BEGIN GPU PROGRAM CREATION
GLuint custom_shaders(const char *vsPath, const char *fsPath)
{
	GLuint vertexShader;
	GLuint fragmentShader;

	vertexShader   = GetShader(GL_VERTEX_SHADER,   vsPath);
	fragmentShader = GetShader(GL_FRAGMENT_SHADER, fsPath);

    shading_program_id = glCreateProgram();

	glAttachShader(shading_program_id, vertexShader);
	glAttachShader(shading_program_id, fragmentShader);

	glLinkProgram(shading_program_id);


	//Error Checking
	GLuint status;
	status=program_check(shading_program_id);
	if (status==GL_FALSE)
		return 0;
	return shading_program_id;

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
