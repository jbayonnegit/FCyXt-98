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
// Fonction de distance pour un Mandelbulb
float fractalSDF(vec3 p)
{
    vec3 z = p;
    float dr = 1.0;
    float r = 0.0;
    
    // La puissance de la fractale (8.0 est la valeur classique)
    // Tu peux essayer de l'animer avec le temps (ex: 8.0 + sin(time) * 2.0)
    float Power = 8.0; 
    
    // Nombre d'itérations de la fractale
    int iterations = 10; 

    for (int i = 0; i < iterations; i++)
    {
        r = length(z);
        if (r > 2.0) break; // Bailout (limite d'échappement)

        // Conversion en coordonnées polaires
        float theta = acos(z.z / r);
        float phi = atan(z.y, z.x);
        
        // Calcul de la dérivée pour l'estimation de distance (Distance Estimator)
        dr = pow(r, Power - 1.0) * Power * dr + 1.0;

        // Mise à l'échelle et rotation
        float zr = pow(r, Power);
        theta = theta * Power;
        phi = phi * Power;

        // Retour aux coordonnées cartésiennes
        z = zr * vec3(sin(theta) * cos(phi), sin(phi) * sin(theta), cos(theta));
        
        // Ajout de la position d'origine
        z += p;
    }
    
    // Formule d'estimation de distance pour le Mandelbulb
    return 0.5 * log(r) * r / dr;
}

float	mapScene(vec3 p)
{
	float d = fractalSDF(vec3(0));

	return (d);
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
			break ;
		if (d > 500.0)  // Trop loin
			break;
		point += d * ray;
	}
	// Calcule la normale seulement si on a trouvé un objet
	if (d < 0.1)
	{
		n = getNormal(point);
		float light2 = dot( n, normalize(vec3(0, 2000, 300) - point ));
		float light0 = dot( n, normalize(vec3(0, 0, -1000) - point ));
		vec4 color = vec4(1.0 * light0 , 0,  1 * light2, 1);
		FragColor = color;
	}
	else
	{
		// Pixel du ciel (pas d'objet trouvé)
		FragColor = vec4(0, 0.002, 0, 1);
	}
}