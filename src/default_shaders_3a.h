static const char common_shader_header[] =
"#version 130\n"
"precision highp float;";

static const char vertex_shader_body[] =
"attribute vec4 iPosition;"
"void main(){gl_Position=iPosition;}";

static const char fragment_shader_header[] =
"uniform vec3 		iResolution;"
"uniform float 		iGlobalTime;"
"uniform float 		iTime;"
"uniform float 		iChannelTime[4];"
"uniform vec4 		iMouse;"
"uniform vec4 		iDate;"
"uniform float 		iSampleRate;"
"uniform vec3 		iChannelResolution[4];"
"uniform sampler2D 	iChannel0;"
"uniform sampler2D 	iChannel1;"
"uniform sampler2D 	iChannel2;"
"uniform sampler2D 	iChannel3;\n";

static const char fragment_shader_footer[] =
"\nvoid main(){mainImage(gl_FragColor,gl_FragCoord.xy);}";

/*
 * Standard ShaderToy Shader
 * after 11/07/2017 - Release 0.9.3
 * https://www.shadertoy.com/new#
 */
static char *default_fragment_shader_1 =
"void mainImage(out vec4 fragColor, in vec2 fragCoord)\n"
"{\n"
"\t//Normalized pixel coordinates (from 0 to 1)\n"
"\tvec2 uv = fragCoord/iResolution.xy;\n"
"\n"
"\t// Time varying pixel color\n"
"\tvec3 col = 0.5 + 0.5*cos(iTime+uv.xyx+vec3(0,2,4));\n"
"\n"
"\t// Output to screen\n"
"\tfragColor = vec4(col,1.0);\n"
"}\n";

/*
 * Standard ShaderToy Shader
 * before 11/07/2017 - Release 0.9.3
*/
static char *default_fragment_shader_0 =
"void mainImage(out vec4 fragColor, in vec2 fragCoord)"
"{"
"vec2 uv=fragCoord.xy/iResolution.xy;"
"fragColor = vec4(uv, 0.5+0.5*sin(iGlobalTime), 1.0);"
"}";

static char *default_fragment_shader =
"#define iterations 17\n"
"#define formuparam 0.53\n"
"#define volsteps 20\n"
"#define stepsize 0.1\n"
"#define zoom   0.800\n"
"#define tile   0.850\n"
"#define speed  0.010\n"
"#define brightness 0.0015\n"
"#define darkmatter 0.300\n"
"#define distfading 0.730\n"
"#define saturation 0.850\n"
"void mainImage(out vec4 fragColor, in vec2 fragCoord)"
"{"
"vec2 uv=fragCoord.xy/iResolution.xy-.5;"
"uv.y*=iResolution.y/iResolution.x;"
"vec3 dir=vec3(uv*zoom,1.);"
"float time=iGlobalTime*speed+.25;"
"float a1=.5+iMouse.x/iResolution.x*2.;"
"float a2=.8+iMouse.y/iResolution.y*2.;"
"mat2 rot1=mat2(cos(a1),sin(a1),-sin(a1),cos(a1));"
"mat2 rot2=mat2(cos(a2),sin(a2),-sin(a2),cos(a2));"
"dir.xz*=rot1;"
"dir.xy*=rot2;"
"vec3 from=vec3(1.,.5,0.5);"
"from+=vec3(time*2.,time,-2.);"
"from.xz*=rot1;"
"from.xy*=rot2;"
"float s=0.1,fade=1.;"
"vec3 v=vec3(0.);"
"for (int r=0; r<volsteps; r++) {"
"vec3 p=from+s*dir*.5;"
"p = abs(vec3(tile)-mod(p,vec3(tile*2.)));"
"float pa,a=pa=0.;"
"for (int i=0; i<iterations; i++) {"
"p=abs(p)/dot(p,p)-formuparam;"
"a+=abs(length(p)-pa);"
"pa=length(p);"
"}"
"float dm=max(0.,darkmatter-a*a*.001);"
"a*=a*a;"
"if (r>6) fade*=1.-dm;"
"v+=vec3(dm,dm*.5,0.);"
"v+=fade;"
"v+=vec3(s,s*s,s*s*s*s)*a*brightness*fade;"
"fade*=distfading;"
"s+=stepsize;"
"}"
"v=mix(vec3(length(v)),v,saturation);"
"fragColor = vec4(v*.01,1.);"
"}";
