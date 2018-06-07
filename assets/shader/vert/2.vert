#version 110
// GLSL         OpenGL 
// 1.10         2.0
in 		vec4 fragColor;
varying out 	vec2 fragCoord;

void main()
{
	gl_Position 	= gl_Vertex;
	fragCoord	= gl_Position.xy;
}
