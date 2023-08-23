// code from 2.vert as array of chars
static const char vertex_shader[] =
"#version 110\n"
"varying vec2 fragCoord;\n"
"void main(){gl_Position = gl_Vertex;fragCoord = gl_Position.xy;}";

// code from 5.frag as array of chars
static const char fragment_shader[] =
"#version 110\n"
"varying vec2  fragCoord;\n"
"uniform float fTime;\n"
"void main(){vec2  uv  = vec2( (fragCoord.xy + 1.0) / 2.0 );vec3  col = 0.5 + 0.5 * cos( fTime + uv.xyx + vec3(0,2,4) );gl_FragColor = vec4(col,1.0);}";
