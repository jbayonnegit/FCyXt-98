#version 410 core

out vec4 FragColor;
uniform vec3	cam_pos;	
uniform vec3	forward;
uniform vec3	spherePos;
uniform vec3	cubePos;
uniform vec3	up;
uniform vec3	right;
uniform vec2	resolution;
uniform vec2	ScreenResolution;

vec3	get_first_ray_direction(void)
{
	vec3	point_on_displayport 	= cam_pos 
								+ forward 
								+ (-up * (resolution.y / 2)) 
								+ (-right * (resolution.x / 2));

	vec2 uv = gl_FragCoord.xy / ScreenResolution;

	point_on_displayport += ( up * (uv.y * resolution.y) ) + ( right * (uv.x * resolution.x) );

	vec3	ray_direction = point_on_displayport - cam_pos;

	return (normalize(ray_direction));
}


void    display_pixel(float x, float y)
{
	vec3 color;
	float u = x;
	float k = y;

	u += 0.5;
	k += 0.5;
	if ((gl_FragCoord.x >= (u - 0.5) && gl_FragCoord.x <= (u + 0.5))
	&& (gl_FragCoord.y >= (k - 0.5) && gl_FragCoord.y <= (k + 0.5)))
		FragColor = vec4( 1.0f, 0.0f ,0.0f ,1.0f);
}

float opSubtraction( float a, float b )
{
    return max(-a,b);
}


float	SdSphere(vec3 p, float r, vec3 pos)
{
	//float d = length(p) - r;
	float d = length(p - pos) - r;
	return (d);
}


float sdBox( vec3 p, vec3 b )
{
	p = p - cubePos;
  vec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}


float opSmoothUnion( float a, float b, float k )
{
    k *= 4.0;
    float h = max(k-abs(a-b),0.0);
    return min(a, b) - h*h*0.25/k;
}

// Grille infinie de sphères avec répétition périodique
float	infiniteSpheres( vec3 p, float sphereRadius, float spacing )
{
	// Répète l'espace pour créer une grille 3D infinie
	vec3 pos = mod(p + spacing/2.0, spacing) - spacing/2.0;
	return length(pos) - sphereRadius;
}

// Alternative : grille 2D infinie (sphères répétées sur le plan XY)
float	infiniteSpheresPlane( vec3 p, float sphereRadius, float spacing )
{
	vec3 pos = p;
	pos.x = mod(pos.x + spacing / 2.0, spacing) - spacing / 2.0;
	pos.y = mod(pos.y + spacing / 2.0, spacing) - spacing / 2.0;
	return length(pos) - sphereRadius;
}


// Mandelbulb - 3D Mandelbrot Set
// Distance estimation using ray marching

// Power function for Mandelbulb
vec3 powN(vec3 z, float n) {
    float r = length(z);
    if (r < 0.0001) return vec3(0.0);
    
    float theta = acos(z.z / r);
    float phi = atan(z.y, z.x);
    
    float rn = pow(r, n);
    float ntheta = theta * n;
    float nphi = phi * n;
    
    return rn * vec3(
        sin(ntheta) * cos(nphi),
        sin(ntheta) * sin(nphi),
        cos(ntheta)
    );
}

// Distance estimation for Mandelbulb
float mandelbulbDE(vec3 pos, float power, int maxIterations) {
    vec3 z = pos;
    float dr = 1.0;
    float r = 0.0;
    
    // Reduced loop limit for better performance
    for (int i = 0; i < 20; i++) {
        if (i >= maxIterations) break;
        r = length(z);
        if (r > 4.0) break;
        
        // Calculate derivative for distance estimation
        dr = pow(r, power - 1.0) * power * dr + 1.0;
        
        // Mandelbulb iteration
        z = powN(z, power) + pos;
    }
    
    // Distance estimate: |z| * log(|z|) / |z'|
    return 0.5 * log(r) * r / dr;
}




float	mapScene(vec3 p)
{
	// float	sphere1 = SdSphere(p ,5, spherePos);
	// float	sphere0 = SdSphere(p ,10, vec3(0));
	// float	box = sdBox(p, vec3(4,4,4));
	//float 	mandel = mandelbulbDE(p, 22, 12);  // radius=20, spacing=100
	float	infiniteSp0 = infiniteSpheres(p, 20, 50);
	//float	infiniteSp1 = infiniteSpheres(p + 20, 30, 100);
	//return (opSmoothUnion(infiniteSp0, infiniteSp1, 2));
	//	return (opSmoothUnion(opSmoothUnion(sphere0, sphere1, 0.5), box, 2));
	return (infiniteSp0);
}

vec3 getNormal(vec3 p)
{
    // Utilise une epsilon plus petite pour les calculs de normale
    const float eps = 0.001;
    
    float d0 = mapScene(p);
    float dx = mapScene(p + vec3(eps, 0, 0)) - d0;
    float dy = mapScene(p + vec3(0, eps, 0)) - d0;
    float dz = mapScene(p + vec3(0, 0, eps)) - d0;
    
    return normalize(vec3(dx, dy, dz));
}

void main ()
{
	vec3 	ray;
	float	d;
	float	normal;

	vec3	n;
	vec3	point = cam_pos;

	ray = get_first_ray_direction();
	// Limite d'itérations pour éviter les boucles infinies
	int maxIterations = 256;
	for (int iter = 0; iter < maxIterations; iter++)
	{
		d = mapScene(point);
		if (d < 0.01)  // Convergence
			break;
		if (d > 500.0)  // Trop loin
			break;
		point += d * ray;
	}
	// Calcule la normale seulement si on a trouvé un objet
	if (d < 0.1)
	{
		n = getNormal(point);
		//float light2 = dot( n, normalize(vec3(0, 2000, 300) - point ));
		float light0 = dot( n, normalize(vec3(0, 0, -1000) - point ));
		vec4 color = vec4(1.0 * light0 , 0,  0, 1);
		FragColor = color;
	}
	else
	{
		// Pixel du ciel (pas d'objet trouvé)
		FragColor = vec4(0, 0.002, 0, 1);
	}
}