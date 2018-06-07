#version 110
// https://www.shadertoy.com/new
in 		vec2  fragCoord;
uniform 	float fTime; 		//Current time in float seconds
varying out 	vec4  fragColor;

float normalize_C(float x, float x_min, float x_max, float r_min, float r_max )
{
	//see https://github.com/Acry/C-math-normalization/blob/master/main.c
	
	// In general, you can always get a new variable xr in
	// [r_min,r_max] when you provide the Dataset [x_min,x_max]

	// xr = (r_max-r_min) * (x - x_min) / (x_max - x_min) + r_min;
	
	float xr;
	xr = (r_max-r_min) * (x - x_min) / (x_max - x_min) + r_min;
	return xr;
}

void main()
{

float x   = normalize_C(fragCoord.x, -1.0, 1.0, 0.0, 1.0);
float y   = normalize_C(fragCoord.y, -1.0, 1.0, 0.0, 1.0);

vec2  uv  = vec2(x, y);
vec3  col = 0.5 + 0.5*cos(fTime+uv.xyx+vec3(0,2,4));

fragColor = vec4(col,1.0);

}
