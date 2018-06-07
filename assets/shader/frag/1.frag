#version 110
#define GL_DARK_ORANGE  0.839, 0.423, 0.274,1
//fixed Color
void main() 
{
	gl_FragColor = vec4(GL_DARK_ORANGE);
	// gl_FragColor is deprecated since GLSL 1.3 (OpenGL 3.0)
	
	// A fragment shader can have multiple outputs gl_FragColor
	// at first was replaced by gl_FragData[i] where i was sent to the
	// i-th enabled draw buffer and these days fragment shader outputs
	// are simply declared as "out" variables, see also:
	// https://www.khronos.org/opengl/wiki/Fragment_Shader

}
