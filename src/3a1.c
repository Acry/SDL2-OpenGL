//BEGIN HEAD
//BEGIN DESCRIPTION
/* 
 * Press 'n' to write new shadertoy file,
 * should open in kate for now.
 * Press 'c' to compile and use if valid
 * 
 * TODO Try inotify interface to monitor file change and try shader compile
 */
//END 	DESCRIPTION

//BEGIN INCLUDES
#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>

#else
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#endif
#include <GL/glew.h>		//No clue how to do Cross-Platform
				//If anyone tries to build tell me
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 	// Just for the icon - easy to strip out
#include "default_shaders_3a.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
//END 	INCLUDES

//BEGIN DEFINES
// TODO Filename and Editor in Config-File
#define NEW_FILE "STFS_new.glsl"
#define EDITOR	"/usr/bin/kate"
//END 	DEFINES

//BEGIN GLOBALS
int ww=500;
int wh=281;

char   Running 		= 1;	// controls main loops
GLuint switch_counter	= 0;	// tracks current used shading program
GLuint argument_provided= 0;

const char editor_call[]={ ""EDITOR" "NEW_FILE };
// NOTE If default_fragment_shader_1 changes, hash needs to be updated!
char hash_current[]="006a3fb6cfb35cc8eaeecf66a4fd166f49c6917e";
char hash_last[]   ="006a3fb6cfb35cc8eaeecf66a4fd166f49c6917e";

GLfloat vertices[] = {
	 -1.0f,   -1.0f,
	  1.0f,   -1.0f,
	 -1.0f,    1.0f,
	  1.0f,    1.0f,
};

//3 from header, one optionally loaded
GLuint shading_program[5];
GLint attrib_position;		// Vertex Shader Attribute

//Used Uniforms so far
GLint uniform_gtime;
GLint uniform_itime;		// Don't need both of them

GLint uniform_res;		// Resolution

//Next one to do
GLint uniform_mouse;		//image/buffer	xy = current pixel coords (if LMB is down). zw = click pixel

//Not used so far
GLint sampler_channel[4];
GLint uniform_cres;
GLint uniform_ctime;
GLint uniform_date;
GLint uniform_srate;
//END  GLOBALS

//BEGIN FUNCTIONS
//BEGIN NEW FILE
// writes def FS from default_fragment_shader_1 to NEW_FILE and calls EDITOR
// first time
int    write_STFS		(void);
// returns if hash changed and keeps track of last hash
int    hash_check		(void);
// tries to compile and use new program if hash was changed
GLuint compile_STFS		(void);
//END 	NEW FILE

// loads custom Vertex- and Fragment Shader from File
GLuint custom_shaders		(const char *	, const char *);

// uses default Vertex-Shader and loads a ST-Shader
GLuint shadertoy_shader		(const char *);

// loads a shader from file and returns the compile shader
GLuint GetShader		(GLenum 	, const char *);

// loads default Vertex- and Fragment Shader if no Arguments are provided
GLuint default_shaders		(GLuint);
GLuint default_vertex		(void);

GLuint compile_shader		(GLenum type, GLsizei , const char **);
GLuint program_check		(GLuint);

void   query_shadertoy_old_vars	(GLuint);
void   query_vars		(GLuint);

void   shader_switch		(void);
float  fTime			(void);
void   init_glew		(void);
const char  *read_file		(const char *);
//END 	FUNCTIONS
//END 	HEAD

//BEGIN MAIN
int main(int argc, const char *argv[])
{
	//BEGIN INIT
	if (argc==2){
		argument_provided=1;
		SDL_Log("%s",argv[1]);
		//for now I will assume type shadertoy is provided
	} else {
		SDL_Log("No argument provided.");
	}
	//BEGIN INIT SDL2
	SDL_Init(SDL_INIT_VIDEO);
	SDL_Window *Window = SDL_CreateWindow("3a1 - New File",
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
		
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);
	SDL_GLContext glContext = SDL_GL_CreateContext(Window);
	//END 	INIT SDL2

	init_glew();

	//BEGIN SHADER INIT

	SDL_Log("Trying to build default shaders");
	for (int i=0; i<3; i++){
		shading_program[i] = default_shaders(i);
		SDL_Log("i: %d", i);
	}
	if (shading_program[0] == 0){
		Running = 0;
		if (glGetError()!=0)
			SDL_Log("glError: %#08x\n", glGetError());
	} else {
		switch(argument_provided)
		{
			case 0:
				glUseProgram(shading_program[0]);
				query_vars(0);
				query_shadertoy_old_vars(0);
				SDL_Log("Using default shader");
				break;
			case 1:
				SDL_Log("Trying to build shadertoy shader");
				shading_program[3] = shadertoy_shader(argv[1]);
				glUseProgram(shading_program[3]);
				SDL_Log("Using shading program %d\n", shading_program[3]);
				query_vars(3);
				query_shadertoy_old_vars(3);
				switch_counter=4;
				break;
			default:
				//some statements to execute when default;
				break;
		}
	}

	glEnableVertexAttribArray	(attrib_position);
	glVertexAttribPointer		(attrib_position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glUniform3f			(uniform_res, ww, wh, 0.0f);
	//END 	SHADER INIT
	SDL_Point mouse;
	char MBL_CLICK=0;
	SDL_Event event;

	//END	INIT

	//BEGIN MAINLOOP
	
	while (Running){
		//BEGIN EVENTS
		while ( SDL_PollEvent(&event) ){
			SDL_GetMouseState(&mouse.x, &mouse.y);
			if (event.type == SDL_QUIT)
				Running = 0;
			if(event.type == SDL_WINDOWEVENT){
				if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED){
					ww=event.window.data1;
					wh=event.window.data2;
					glViewport (0, 0, ww, wh);
					glUniform3f(uniform_res, ww, wh, 0.0f);
				}
			}
			if(event.type == SDL_MOUSEMOTION){
				;
			}
			if(event.type == SDL_MOUSEBUTTONDOWN){
				if(event.button.button == SDL_BUTTON_RIGHT){
					;
				}
				if(event.button.button == SDL_BUTTON_MIDDLE){
					shader_switch();
				}
				if( event.button.button==SDL_BUTTON_LEFT){
					MBL_CLICK=1;
				}
			}
			if(event.type == SDL_MOUSEBUTTONUP){
				if( event.button.button==SDL_BUTTON_LEFT){
					MBL_CLICK=0;
				}	
			}
			if(event.type == SDL_KEYDOWN ){
				switch(event.key.keysym.sym ){
					case SDLK_ESCAPE:
						Running = 0;
						break;
					case SDLK_n:
						write_STFS();
						break;
					case SDLK_c:
						compile_STFS();
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
		//END 	EVENTS

		//BEGIN UPDATE
		if (MBL_CLICK==1)
			glUniform4f(uniform_mouse, mouse.x, mouse.y, 0, 0 );

		glUniform1f(uniform_gtime, fTime());

		//END UPDATE
		//BEGIN RENDERING
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		SDL_GL_SwapWindow(Window);
		//END 	RENDERING
	}
	//END 	MAINLOOP

	//BEGIN EXIT
	for (int i=0; i<4; i++){
		if (glIsProgram(shading_program[i]))
			glDeleteProgram(shading_program[i]);
	}
	SDL_GL_DeleteContext(glContext);
	SDL_Quit();

	return EXIT_SUCCESS;
	//END 	EXIT
}
//END	MAIN

//BEGIN FUNCTIONS

//BEGIN GPU PROGRAM CREATION
GLuint custom_shaders(const char *vsPath, const char *fsPath)
{
	GLuint vertexShader;
	GLuint fragmentShader;

	vertexShader   = GetShader(GL_VERTEX_SHADER,   vsPath);
	fragmentShader = GetShader(GL_FRAGMENT_SHADER, fsPath);

	shading_program[3] = glCreateProgram();

	glAttachShader(shading_program[3], vertexShader);
	glAttachShader(shading_program[3], fragmentShader);

	glLinkProgram(shading_program[3]);

	GLuint status;
	status=program_check(shading_program[3]);
	if (status==GL_FALSE)
		return 0;
	return shading_program[3];

}

GLuint shadertoy_shader(const char *fsPath)
{
	GLuint vtx;
	vtx = default_vertex();
	if (vtx==0)
		return 0;

	GLuint frag;
	const char *sources[4];
	sources[0] = common_shader_header;
	sources[1] = fragment_shader_header;
	sources[2] = read_file(fsPath);
	sources[3] = fragment_shader_footer;
	frag = compile_shader(GL_FRAGMENT_SHADER, 4, sources);
	
	shading_program[3] = glCreateProgram();
	
	glAttachShader(shading_program[3], vtx);
	glAttachShader(shading_program[3], frag);
	
	glLinkProgram(shading_program[3]);
	
	GLuint status;
	status=program_check(shading_program[3]);
	if (status==GL_FALSE)
		return 0;
	return shading_program[3];
	
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
	return shader;
}

GLuint default_shaders(GLuint choice)
{
	SDL_Log("choice def: %d", choice);
	GLuint vtx;
	vtx = default_vertex();
	if (vtx==0)
		return 0;
	
	GLuint frag;
	const char *sources[4];
	sources[0] = common_shader_header;
	sources[1] = fragment_shader_header;
	switch(choice)
	{
		case 0:
			sources[2] = default_fragment_shader_0;
			break;
		case 1:
			sources[2] = default_fragment_shader_1;
			break;
		case 2:
			sources[2] = default_fragment_shader;
			break;
		default:
			//some statements to execute when default;
			break;
	}
	
	sources[3] = fragment_shader_footer;
	frag = compile_shader(GL_FRAGMENT_SHADER, 4, sources);

	shading_program[choice] = glCreateProgram();
	glAttachShader(shading_program[choice], vtx);
	glAttachShader(shading_program[choice], frag);
	glLinkProgram(shading_program[choice]);


	GLuint status;
	status=program_check(shading_program[choice]);
	if (status==GL_FALSE)
		return 0;

	return shading_program[choice];
}


GLuint default_vertex(void)
{
	GLuint vtx;
	const char *sources[2];
	sources[0] = common_shader_header;
	sources[1] = vertex_shader_body;
	vtx = compile_shader(GL_VERTEX_SHADER, 2, sources);
	return vtx;
	
}

GLuint program_check(GLuint program)
{

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

void query_shadertoy_old_vars(GLuint choice)
{

	sampler_channel[0] = glGetUniformLocation(shading_program[choice], "iChannel0");
	sampler_channel[1] = glGetUniformLocation(shading_program[choice], "iChannel1");
	sampler_channel[2] = glGetUniformLocation(shading_program[choice], "iChannel2");
	sampler_channel[3] = glGetUniformLocation(shading_program[choice], "iChannel3");
	uniform_cres  = glGetUniformLocation(shading_program[choice], "iChannelResolution");
	uniform_ctime = glGetUniformLocation(shading_program[choice], "iChannelTime");
	uniform_itime = glGetUniformLocation(shading_program[choice], "iTime");
	uniform_res   = glGetUniformLocation(shading_program[choice], "iResolution");
	uniform_srate = glGetUniformLocation(shading_program[choice], "iSampleRate");
}

void query_vars(GLuint choice)
{

	GLint i;
	GLint count;
	
	GLint size;
	GLenum type;
	
	const GLsizei bufSize = 16;
	GLchar name[bufSize];
	GLsizei length;

	glGetProgramiv(shading_program[choice], GL_ACTIVE_ATTRIBUTES, &count);

	
	for (i = 0; i < count; i++){
		glGetActiveAttrib(shading_program[choice], (GLuint)i, bufSize, &length, &size, &type, name);
		if (!strcmp("iPosition",name)){
			attrib_position = i;
		}

	}
	

	glGetProgramiv(shading_program[choice], GL_ACTIVE_UNIFORMS, &count);
	
	char global_time1[]="iTime";
	char global_time2[]="iGlobalTime";
	for (i = 0; i < count; i++){
		glGetActiveUniform(shading_program[choice], (GLuint)i, bufSize, &length, &size, &type, name);
		
		SDL_Log("Uniform #%d Type: %u Name: %s\n", i, type, name);
		if (!strcmp(global_time1,name)||!strcmp(global_time2,name)){
			uniform_gtime = i;
		}
		if (!strcmp("iMouse",name))
			uniform_mouse = i;

	}
}
//BEGIN MISC
float fTime(void)
{

	static Uint64 start 	 = 0;
	static Uint64 frequency  = 0;
	
	if (start==0){
		start		=	SDL_GetPerformanceCounter();
		frequency	=	SDL_GetPerformanceFrequency();
		return 0.0f;
	}
	Uint64 counter    = 0;
	       counter    = SDL_GetPerformanceCounter();
	Uint64 accumulate = counter - start;
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
	
	if (!GLEW_VERSION_3_0){
		SDL_Log("At least OpenGL 3.0 with GLSL 1.30 required.");
		Running = 0;
	}
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

int write_STFS(void)
{

	FILE *fp;
	fp = fopen( ""NEW_FILE , "w" );
	fputs(default_fragment_shader_1, fp);
	SDL_Log("Shader written");
	fclose(fp);
	system(editor_call);
	switch_counter=0;
	shader_switch();
	return(0);

}

GLuint compile_STFS(void)
{

if (hash_check()){
// 	SDL_Log("hash changed");
	GLuint vtx;
	vtx = default_vertex();
	if (vtx==0)
		return 0;
	
	GLuint frag;
	const char *sources[4];
	sources[0] = common_shader_header;
	sources[1] = fragment_shader_header;
	sources[2] = read_file(""NEW_FILE);
	sources[3] = fragment_shader_footer;
	frag = compile_shader(GL_FRAGMENT_SHADER, 4, sources);
	
	shading_program[4] = glCreateProgram();
	
	glAttachShader(shading_program[4], vtx);
	glAttachShader(shading_program[4], frag);
	
	glLinkProgram(shading_program[4]);
	
	GLuint status;
	status=program_check(shading_program[4]);
	if (status==GL_FALSE){
		return 0;
	}else{
		glUseProgram(shading_program[4]);
		SDL_Log("own_shading_program: %d", shading_program[4]);
		query_vars(4);
		query_shadertoy_old_vars(4);
		glEnableVertexAttribArray	(attrib_position);
		glVertexAttribPointer		(attrib_position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		
		glViewport (0, 0, ww, wh);
		glUniform3f(uniform_res, ww, wh, 0.0f);
		return 1;
	}
	
}

return 0;

}

int hash_check(void)
{
	//GET HASH
	FILE *pipe_in;
	char pipe_buff[99];
	
	if((pipe_in = popen("sha1sum "NEW_FILE, "r")) == NULL){
		perror("popen");
		exit(1);
	}
	
	while(fgets(pipe_buff, sizeof(pipe_buff), pipe_in)!=NULL){
// 		SDL_Log("hash: %s", pipe_buff);
	}
	
	size_t n=40;
	strncpy(hash_current, pipe_buff, n);
	hash_current[40] = '\0';
	pclose(pipe_in);
	

	if ( strcmp(hash_last,hash_current) ){
		strcpy(hash_last,hash_current);
		return 1;
	} else {
		return 0;
	}
}

void shader_switch(void)
{
	switch_counter++;
	if (switch_counter>(2+argument_provided))
		switch_counter=0;
	SDL_Log("switch_counter: %d", switch_counter);
	glUseProgram(shading_program[switch_counter]);
	query_vars(switch_counter);
	query_shadertoy_old_vars(switch_counter);
	glEnableVertexAttribArray	(attrib_position);
	glVertexAttribPointer		(attrib_position, 2, GL_FLOAT, GL_FALSE, 0, vertices);

	glViewport (0, 0, ww, wh);
	glUniform3f(uniform_res, ww, wh, 0.0f);
}
//END 	MISC

//END FUNCTIONS
