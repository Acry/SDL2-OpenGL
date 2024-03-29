//BEGIN HEAD
//BEGIN DESCRIPTION

/*
 * Add inotify and epoll to detect changes in file and recompile shader.
 *
 * Press 'n' to write a default fragment shader in shadertoy-style to file.
 * #define NEW_FILE "STFS_new.glsl" - using the suffix for syntax-hilighting.
 * 
 * It should open in vscode for now, but should work with every editor.
 * #define EDITOR	"/usr/bin/vscode"
 * 
 * Changing and saving in Editor should be enough to call compile.
 * 'c' to compile is still valid.
 * 
 * Introduced function reset_poll using epoll and inotify.
 * Should be faster than STAT(2).
 * 
 * TODO: Filename and Editor in Config-File
 * 
 * So basically one can edit on the fly now or copy and paste shadercode
 * from shadertoy. With save-as in editor work is permanent.
 */

//END 	DESCRIPTION

//BEGIN INCLUDES

// gave up on cross-platform for now
#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h> 	// Just for the icon - easy to strip out
#include "default_shaders_3a.h" // Contains def vertex and frag shader

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
// #include <errno.h>
#include <sys/inotify.h>
#include <sys/epoll.h>

//END 	INCLUDES

//BEGIN DEFINES

#define NEW_FILE "STFS_new.glsl"
#define EDITOR	"/usr/bin/code"

//END 	DEFINES

//BEGIN GLOBALS

int ww=500;
int wh=281;

char   Running 		= 1;	// controls main loops
GLuint switch_counter	= 0;	// tracks current used shading program
GLuint argument_provided= 0;

int NEW_SHADER=0;

//inotify and epoll
int wd, fd, efd, cfg;

struct epoll_event ev;

const char editor_call[]={ ""EDITOR" "NEW_FILE };

GLfloat vertices[] = {
	-1.0f,   -1.0f,
	 1.0f,   -1.0f,
	-1.0f,    1.0f,
	 1.0f,    1.0f,
};

//3 from header, one optionally loaded
GLuint shading_program_id[5];

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

struct epoll_event reset_poll(int);
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
	SDL_Window *Window = SDL_CreateWindow("3a2 - compile on save",
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

	//END 	INIT SDL2

	init_glew();

	//BEGIN SHADER INIT

	SDL_Log("Trying to build default shaders");
	for (int i=0; i<3; i++){
        shading_program_id[i] = default_shaders(i);
		SDL_Log("i: %d", i);
	}
	if (shading_program_id[0] == 0){
		Running = 0;
		if (glGetError()!=0)
			SDL_Log("glError: %#08x\n", glGetError());
	} else {
		switch(argument_provided)
		{
			case 0:
				glUseProgram(shading_program_id[0]);
				query_vars(0);
				query_shadertoy_old_vars(0);
				SDL_Log("Using default shader");
				break;
			case 1:
				SDL_Log("Trying to build shadertoy shader");
                shading_program_id[3] = shadertoy_shader(argv[1]);
				glUseProgram(shading_program_id[3]);
				SDL_Log("Using shading program %d\n", shading_program_id[3]);
				query_vars(3);
				query_shadertoy_old_vars(3);
				switch_counter=4;
				break;
			default:
				break;
		}
	}

	glEnableVertexAttribArray	(attrib_position);
	glVertexAttribPointer		(attrib_position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
	glUniform3f			(uniform_res, ww, wh, 0.0f);

	//END 	SHADER INIT

	SDL_Point mouse;
	char MBL_CLICK=0;
	struct epoll_event ev;
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
                    ww = event.window.data1;
                    wh = event.window.data2;
					glViewport (0, 0, ww, wh);
					glUniform3f(uniform_res, (float)ww, (float)wh, 0.0f);
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
						ev = reset_poll(0);
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

		if (NEW_SHADER==1){
			if (epoll_wait(efd, &ev, sizeof(ev), 0) > 0){
				SDL_Log("try compile");
				compile_STFS();
				ev = reset_poll(0);
			}
		}
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

	if (NEW_SHADER==1){
		ev = reset_poll(1);
	}

	for (int i=0; i<4; i++){
		if (glIsProgram(shading_program_id[i]))
			glDeleteProgram(shading_program_id[i]);
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

    shading_program_id[3] = glCreateProgram();

	glAttachShader(shading_program_id[3], vertexShader);
	glAttachShader(shading_program_id[3], fragmentShader);

	glLinkProgram(shading_program_id[3]);

	GLuint status;
	status=program_check(shading_program_id[3]);

	if (status==GL_FALSE) return 0;

	return shading_program_id[3];
}

GLuint shadertoy_shader(const char *fsPath)
{
	GLuint vtx;
	vtx = default_vertex();
	if (vtx==0)	return 0;

	GLuint frag;
	const char *sources[4];
	sources[0] = common_shader_header;
	sources[1] = fragment_shader_header;
	sources[2] = read_file(fsPath);
	sources[3] = fragment_shader_footer;
	frag = compile_shader(GL_FRAGMENT_SHADER, 4, sources);

    shading_program_id[3] = glCreateProgram();

	glAttachShader(shading_program_id[3], vtx);
	glAttachShader(shading_program_id[3], frag);

	glLinkProgram(shading_program_id[3]);

	GLuint status;
	status=program_check(shading_program_id[3]);

	if (status==GL_FALSE) return 0;

	return shading_program_id[3];
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

GLuint default_shaders(GLuint choice)
{
	SDL_Log("choice def: %d", choice);
	GLuint vtx;
	vtx = default_vertex();
	if (vtx==0) return 0;

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
			break;
	}

	sources[3] = fragment_shader_footer;
	frag = compile_shader(GL_FRAGMENT_SHADER, 4, sources);

    shading_program_id[choice] = glCreateProgram();
	glAttachShader(shading_program_id[choice], vtx);
	glAttachShader(shading_program_id[choice], frag);
	glLinkProgram(shading_program_id[choice]);

	//Error Checking
	GLuint status;
	status=program_check(shading_program_id[choice]);

	if (status==GL_FALSE) return 0;

	return shading_program_id[choice];
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
	sampler_channel[0] = glGetUniformLocation(shading_program_id[choice], "iChannel0");
	sampler_channel[1] = glGetUniformLocation(shading_program_id[choice], "iChannel1");
	sampler_channel[2] = glGetUniformLocation(shading_program_id[choice], "iChannel2");
	sampler_channel[3] = glGetUniformLocation(shading_program_id[choice], "iChannel3");
	uniform_cres  = glGetUniformLocation(shading_program_id[choice], "iChannelResolution");
	uniform_ctime = glGetUniformLocation(shading_program_id[choice], "iChannelTime");
	uniform_itime = glGetUniformLocation(shading_program_id[choice], "iTime");
	uniform_res   = glGetUniformLocation(shading_program_id[choice], "iResolution");
	uniform_srate = glGetUniformLocation(shading_program_id[choice], "iSampleRate");
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

	glGetProgramiv(shading_program_id[choice], GL_ACTIVE_ATTRIBUTES, &count);

	for (i = 0; i < count; i++){
		glGetActiveAttrib(shading_program_id[choice], (GLuint)i, bufSize, &length, &size, &type, name);
		if (!strcmp("iPosition",name)){
			attrib_position = i;
		}
	}

	glGetProgramiv(shading_program_id[choice], GL_ACTIVE_UNIFORMS, &count);

	char global_time1[]="iTime";
	char global_time2[]="iGlobalTime";
	for (i = 0; i < count; i++){
		glGetActiveUniform(shading_program_id[choice], (GLuint)i, bufSize, &length, &size, &type, name);
		
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
    Uint64 counter    = SDL_GetPerformanceCounter();
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
	NEW_SHADER=1;
	system(editor_call);
	switch_counter=0;
	shader_switch();

	return(0);
}

struct epoll_event reset_poll(int halt)
{
	static struct epoll_event ev;
	if (halt){
		// release epoll
		// NOTE One can't rely on close to clean up epoll subscriptions. 
		// NOTE One always must always explicitly call epoll_ctl(EPOLL_CTL_DEL)
		//      before calling close().
		epoll_ctl(efd, EPOLL_CTL_DEL, fd, &ev);
		close(efd);
		
		// release inotify
		inotify_rm_watch( fd, wd );
		close(wd);
		close(fd);
		return ev;
	}
	
	fd = inotify_init();
	wd = inotify_add_watch(fd, ""NEW_FILE, IN_CLOSE_WRITE);
	ev.events = EPOLLIN|EPOLLOUT|EPOLLONESHOT;
	efd = epoll_create(sizeof(fd));
	epoll_ctl(efd, EPOLL_CTL_ADD, fd, &ev);
	return ev;
}

GLuint compile_STFS(void)
{
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

    shading_program_id[4] = glCreateProgram();
	
	glAttachShader(shading_program_id[4], vtx);
	glAttachShader(shading_program_id[4], frag);
	
	glLinkProgram(shading_program_id[4]);
	
	GLuint status;
	status=program_check(shading_program_id[4]);

	if (status==GL_FALSE){
		return 0;
	}else{
		glUseProgram(shading_program_id[4]);
		SDL_Log("own_shading_program: %d", shading_program_id[4]);
		query_vars(4);
		query_shadertoy_old_vars(4);
		glEnableVertexAttribArray	(attrib_position);
		glVertexAttribPointer		(attrib_position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
		
		glViewport (0, 0, ww, wh);
		glUniform3f(uniform_res, ww, wh, 0.0f);
		return 1;
	}
}

void shader_switch(void)
{
	switch_counter++;
	if (switch_counter>(2+argument_provided)) switch_counter = 0;
	SDL_Log("switch_counter: %d", switch_counter);
	glUseProgram(shading_program_id[switch_counter]);
	query_vars(switch_counter);
	query_shadertoy_old_vars(switch_counter);
	glEnableVertexAttribArray	(attrib_position);
	glVertexAttribPointer		(attrib_position, 2, GL_FLOAT, GL_FALSE, 0, vertices);
    glUniform3f(uniform_res, (float)ww, (float)wh, 0.0f);
	glViewport (0, 0, ww, wh);
}

//END 	MISC

//END FUNCTIONS
