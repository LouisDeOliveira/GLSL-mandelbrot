#ifdef GL_ES
precision highp float;
#endif

uniform float u_xmax;
uniform float u_xmin;
uniform float u_ymax;
uniform float u_ymin;
uniform int u_color;
uniform vec2 u_resolution;
int N = 1000;
float radius = 0.05;

vec3 bernsteinRBG(float t) {
  return vec3(9.0 * (1.0 - t) * t * t,
			  15.0 * (1.0 - t) * (1.0 - t) * t * t,
			  8.0 * (1.0 - t) * (1.0 - t) * (1.0 - t) * t);
}

vec3 altRGB(int t){
	return vec3(
		0.5 * sin(0.1 * (float)t) + 0.5,
		0.5 * sin(0.1 * (float)t + 2.094) + 0.5,
		0.5 * sin(0.1 * (float)t + 4.188) + 0.5
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

vec3 get_color(int it, int u_color){
	if (u_color == 0){
		return bernsteinRBG(float(it) / float(N));
	}
	else{
		return altRGB(it);
	}
}


void main( void ) {	
	//normalized image coordinates (from -1 to 1)
	vec2 uv = gl_FragCoord.xy / u_resolution.xy;

	//normalized mouse coordinates (from 0 to 1)

	//compute the center of the image and compute the mandelbrot set
	vec2 p = computeCenter(uv);
	float zoom = (u_xmax - u_xmin) / 2.0;
	vec2 dc = uv*zoom;
	int it = mandelbrot(p);
	vec3 color;
	color = get_color(it, u_color);

	gl_FragColor = vec4(color, 1.0);
	
}