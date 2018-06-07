static const char vertex_shader[] =
"#version 110\n"
"in vec4 fragColor;"
"varying out vec2 fragCoord;"
"void main(){gl_Position = gl_Vertex;fragCoord = gl_Position.xy;}";

static const char fragment_shader[] =
"#version 110\n"
"in vec2  fragCoord;"
"uniform float fTime;"
"varying out vec4  fragColor;"
"void main(){vec2  uv  = vec2( (fragCoord.xy - -1.0) / 2.0 );vec3  col = 0.5 + 0.5 * cos( fTime + uv.xyx + vec3(0,2,4) );fragColor = vec4(col,1.0);}";
