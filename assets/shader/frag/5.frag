#version 110
// compare to https://www.shadertoy.com/new
varying 	vec2  fragCoord;
uniform 	float fTime; 	//Current time in float seconds

void main()
{
    // simplified range switch from -1..1 to 0..1
    vec2  uv  = (fragCoord.xy + 1.0) / 2.0;
    vec3  col = 0.5 + 0.5 * cos( fTime + uv.xyx + vec3(0,2,4) );
    gl_FragColor = vec4(col,1.0);
}
