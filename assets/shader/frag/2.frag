#version 110

varying 		vec2 fragCoord;

// Normalize NDC to range [0,1]
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
	float y = normalize_C(fragCoord.y, -1.0, 1.0, 0.0, 1.0);

	gl_FragColor = vec4(x,y,1.0-x,1.0);
}
