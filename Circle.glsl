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

float sdPlane( vec3 p, vec3 n, float h )
{
  // n must be normalized
  return dot(p,n) + h;
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




// Fonctions utilitaires pour la Mandelbox (à placer au-dessus de mapScene)
void sphereFold(inout vec3 z, inout float dz) {
	float r2 = dot(z, z);
	if (r2 < 0.25) { 
		float temp = 4.0;
		z *= temp;
		dz *= temp;
	} else if (r2 < 1.0) { 
		float temp = 1.0 / r2;
		z *= temp;
		dz *= temp;
	}
}

void boxFold(inout vec3 z) {
	z = clamp(z, -1.0, 1.0) * 2.0 - z;
}

// La scène
float mapScene(vec3 p)
{
	float	sizeOfTheFractal = 1;

	vec3 z = p / sizeOfTheFractal;
	vec3 offset = p;
	float dr = 1.0;
	float scale = 2; // Modifie cette valeur (ex: 2.5 ou -1.5) pour changer radicalement la fractale !
	
	for (int n = 0; n < 12; n++) {
		boxFold(z);
		sphereFold(z, dr);
		
		z = scale * z + offset;
		dr = dr * abs(scale) + 1.0;
	}
	
	float r = length(z);
	
	return opSmoothUnion(r / abs(dr) * sizeOfTheFractal, sdPlane(p, vec3(0,0,1), 0.2), 0.23);
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

struct s_res
{
	float d;
	vec3 point;
};

s_res   is_Intersect( vec3 ray_dir, vec3 origin)
{
	float	d;
	vec3	point = origin;
	s_res result;

	int maxIterations = 600;
	for (int iter = 0; iter < maxIterations; iter++)
	{
		d = mapScene(point);
		if (d <= 0.01)  // Convergence
			break;
		if (d > 5000)  // Trop loin
			break;
		point += d * ray_dir;
	}
	result.d = d;
	result.point = point;
	return (result);
}

bool    isHardShadow( vec3 ray, vec3 origin )
{
	s_res result_hard = is_Intersect( ray, origin );
	if ( result_hard.d <= 0.01 )
		return ( true );
	else
		return ( false );
}

void main ()
{
	vec3 	ray;
	float	d;
	s_res result;
	vec3	n;

	ray = get_first_ray_direction();
	result = is_Intersect( ray, cam_pos);
	if (result.d <=  0.01)
	{
		vec3    light_pos = vec3(0, 100, 20);
		vec3    to_light  =  normalize( light_pos - result.point);
		
		n = getNormal(result.point);
		if ( isHardShadow(to_light, result.point + n * 0.02) )
		{
			FragColor = vec4( 0, 0, 0, 1);
			return;
		}
		float light0 = max(0.0, dot( n, to_light));
		vec4 color = /*vec4( 1.0 * light0, 0, 0, 1) +*/ vec4(1 * light0  , 1 * light0,  1 * light0, 1);// + vec4(0 , 1 * light2,  0, 1);
		FragColor = color;
	}
	else
		FragColor = vec4(0, 0, 0.4, 1);
}