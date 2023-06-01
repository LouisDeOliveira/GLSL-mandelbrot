#ifdef GL_ES
precision highp float;
#endif

uniform float u_xmax;
uniform float u_xmin;
uniform float u_ymax;
uniform float u_ymin;
uniform vec2 u_resolution;
int N = 10000;
float radius = 0.05;

vec3 bernsteinRBG(float t) {
  return vec3(9.0 * (1.0 - t) * t * t,
			  15.0 * (1.0 - t) * (1.0 - t) * t * t,
			  8.0 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t);
}

vec3 altRGB(int t){
	return vec3(
		0.5f * sin(0.1f * (float)t) + 0.5f,
		0.5f * sin(0.1f * (float)t + 2.094f) + 0.5f,
		0.5f * sin(0.1f * (float)t + 4.188f) + 0.5f
	)
;}

int mandelbrot(vec2 c) {
  vec2 z = vec2(0.0, 0.0);
  int i = 0;
  for (i = 0; i < N; i++) {
	z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
	if (length(z) > 2.0) {
	  break;
	}
  }
  return i;
}

vec2 computeCenter(vec2 uv){
	vec2 center = vec2(uv.x * (u_xmax - u_xmin) + u_xmin, uv.y * (u_ymax - u_ymin)  + u_ymin);
	return center;
}




void main( void ) {	
	//normalized image coordinates (from -1 to 1)
	vec2 uv = gl_FragCoord.xy / u_resolution.xy;

	//normalized mouse coordinates (from 0 to 1)

	//compute the center of the image and compute the mandelbrot set
	int it = mandelbrot(computeCenter(uv));

	vec3 color = altRGB(it);


	gl_FragColor = vec4( color, 1.0);
	
}