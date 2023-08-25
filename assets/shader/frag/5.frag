#version 110
// compare to https://www.shadertoy.com/new
varying 	vec2  fragCoord;
uniform 	float fTime; 	//Current time in float seconds

void main()
{
    // simplified range switch from -1..1 to 0..1
    vec2  uv  = (fragCoord.xy + 1.0) / 2.0;
    // It transforms the fragCoord values from clip space to a normalized space.
    //
    // In clip space, coordinates are typically in the range [-1.0, 1.0] for both x and y components.
    // The line of code takes each component of fragCoord, adds 1 to it, and then divides by 2.
    //
    // fragCoord.xy + 1.0: This operation shifts the range from [-1.0, 1.0] to [0.0, 2.0].
    // Dividing by 2 (/ 2.0): This scales the range down to [0.0, 1.0].
    // So, after this transformation, uv will contain values in a normalized range [0.0, 1.0] for both x and y
    // components.
    //
    // This type of transformation is common when you want to map clip space coordinates to texture coordinates
    // (or any other normalized space). In this normalized space, (0.0, 0.0) corresponds to the bottom-left corner,
    // and (1.0, 1.0) corresponds to the top-right corner.
    //
    // It allows you to easily sample a texture or perform computations in a standardized coordinate space,
    // where you're sure that values will always be between 0 and 1 regardless of their position on the screen.

    vec3  col = 0.5 + 0.5 * cos( fTime + uv.xyx + vec3(0,2,4) );
    gl_FragColor = vec4(col,1.0);
}
