#version 110
// https://www.shadertoy.com/new
in 		vec2  fragCoord;
uniform 	float fTime; 		//Current time in float seconds
varying out 	vec4  fragColor;

void main()
{

// vec2  uv  = vec2( (fragCoord.x - -1.0) / 2.0, (fragCoord.y -1.0) / 2.0 );
vec2  uv  = vec2( (fragCoord.xy - -1.0) / 2.0 );
vec3  col = 0.5 + 0.5 * cos( fTime + uv.xyx + vec3(0,2,4) );
fragColor = vec4(col,1.0);

}
