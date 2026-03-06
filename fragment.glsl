#version 410 core
#extension GL_ARB_gpu_shader_fp64 : require

out vec4 FragColor;
uniform dvec2 u_resolution;
uniform double zoom;
uniform int limit;
uniform double offsetX;
uniform double cr;
uniform double offsetY;

// Palette couleur rapide
dvec3 get_color(int it)
{
	if (it == limit)
		return dvec3(0.0);

	float t = float(it) * 0.02;
	vec3 col = 0.5 + 0.5 * cos(6.283185 * (t + vec3(0.0, 0.33, 0.67)));
	return dvec3(col);
}

void main()
{
	// Conversion coordonnées écran -> Mandelbrot
	double x = -2.0 * 1.77777777778 + (gl_FragCoord.x * zoom + offsetX) / u_resolution.x * (4.0 * 1.77777777778);
	double y = -2.0 + (gl_FragCoord.y * zoom + offsetY) / u_resolution.y * 4.0;
	
	// Initialisation
	double zr = x;
	double zi = y;
	double cr_val = x;
	double ci_val = y;
	int it = 0;
	
	// Boucle Mandelbrot optimisée
	while (it < limit)
	{
		// Calcul z² + c avec inline
		double zr2 = zr * zr;
		double zi2 = zi * zi;
		double mag2 = zr2 + zi2;
		
		// Early exit si diverge
		if (mag2 > 4.0)
		{
			FragColor = vec4(get_color(it), 1.0);
			return;
		}
		
		// z = z² + c
		zi = 2.0 * zr * zi + ci_val;
		zr = zr2 - zi2 + cr_val;
		
		it++;
	}
	
	// Point dans l'ensemble
	FragColor = vec4(0.0, 0.0, 0.0, 1.0);
} 