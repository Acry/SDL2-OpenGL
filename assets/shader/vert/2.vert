#version 110
// - OpenGL 2.0, GLSL: #version 110 - 2004
// - OpenGL 2.1, GLSL: #version 120 - 2006

// Clip-space coordinates
varying vec2 fragCoord;

void main()
{
	gl_Position = gl_Vertex;
	// Clip-space coordinates.
	// A normalized representation of vertex position in the 3D scene
	// (before being mapped to actual screen pixels). (gl_FragCoord)
	// vertex data is givven in range -1.0 to 1.0.
	fragCoord = gl_Position.xy;
}
