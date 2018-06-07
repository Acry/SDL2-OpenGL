#version 110
in 		vec2  fragCoord;
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

float x = normalize_C(fragCoord.x, -1.0, 1.0, 0.0, 1.0);
fragColor.r=x;
fragColor.g=0.0;
fragColor.b=0.0;
fragColor.a=1.0;

}
