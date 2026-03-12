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

// Calcule l'angle en degrés entre deux vecteurs
float getAngleDegrees(vec3 v1, vec3 v2) {
	// On normalise les vecteurs et on limite le produit scalaire entre -1.0 et 1.0
	// pour éviter les erreurs NaN avec acos dues aux imprécisions flottantes.
	return degrees(acos(clamp(dot(normalize(v1), normalize(v2)), -1.0, 1.0)));
}

// Rapporte la taille d'un vecteur entre 0.0 et 1.0 en fonction d'une taille maximale
float convertRange(vec3 v, float maxLength) {
	// Simple produit en croix pour obtenir la proportion
	return length(v) / maxLength;
}

// Variable globale pour stocker l'information de coloration (Orbit Trap)
vec4 trap = vec4(1000.0);

// La scène
float mapScene(vec3 p)
{
	// Réinitialise le trap pour chaque rayon
	trap = vec4(1000.0);
	
	// Répétition infinie sur les axes XYZ
	float spacing = 10.0;
	vec3 q = p;
	q.x = mod(q.x + spacing / 2.0, spacing) - spacing / 2.0;
	q.y = mod(q.y + spacing / 2.0, spacing) - spacing / 2.0;
	q.z = mod(q.z + spacing / 2.0, spacing) - spacing / 2.0;

	float	sizeOfTheFractal = 1.0;

	vec3 z = q / sizeOfTheFractal;
	vec3 offset = q;
	float dr = 1.0;
	float scale = 2.5; // Modifie cette valeur (ex: 2.5 ou -1.5) pour changer radicalement la fractale !
	
	for (int n = 0; n < 12; n++) {
		boxFold(z);
		sphereFold(z, dr);
		
		z = scale * z + offset;
		dr = dr * abs(scale) + 1.0;
		
		// Orbit Trapping : on garde la trace des distances minimales successives
		// ça servira à donner de la couleur aux formes !
		trap = min(trap, vec4(abs(z), length(z)));
	}
	
	float r = length(z);
	
	// On retourne juste la distance à la fractale (le plan masquerait les fractales du bas)
	return r / abs(dr) * sizeOfTheFractal;
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
	float glow; // Nouvelle propriété pour l'aura lumineuse
};

s_res   is_Intersect( vec3 ray_dir, vec3 origin)
{
	float	d;
	vec3	point = origin;
	s_res result;
	float accumulatedGlow = 0.0;

	int maxIterations = 600;
	for (int iter = 0; iter < maxIterations; iter++)
	{
		d = mapScene(point);
		
		// Accumulation de Glow si on passe tout près d'une surface
		// On ajoute plus de glow si la distance (d) est petite
		accumulatedGlow += (0.005 / (0.01 + abs(d)));
		
		if (d <= 0.001)  // Convergence
			break;
		if (d > 5000.0)  // Trop loin
			break;
		point += d * ray_dir;
	}
	result.d = d;
	result.point = point;
	result.glow = accumulatedGlow; // On sauvegarde le glow calculé
	return (result);
}

// --- Ambient Occlusion ---
float calcAO(vec3 pos, vec3 nor)
{
	float occ = 0.0;
	float sca = 1.0;
	for(int i = 0; i < 5; i++)
	{
		float h = 0.01 + 0.15 * float(i) / 4.0;
		float d = mapScene(pos + h * nor);
		occ += (h - d) * sca;
		sca *= 0.95;
	}
	return clamp(1.0 - 3.0 * occ, 0.0, 1.0);
}

// --- Soft Shadow ---
// ro = ray origin (point on surface + small offset normal)
// rd = ray direction (towards light)
// mint = minimum distance to start checking (avoid self-shadow)
// maxt = maximum distance (e.g. distance to light)
// k = softness factor (higher = harder shadow)
float calcSoftShadow(vec3 ro, vec3 rd, float mint, float maxt, float k)
{
	float res = 1.0;
	float t = mint;
	for(int i = 0; i < 64 && t < maxt; i++)
	{
		float h = mapScene(ro + rd * t);
		if(h < 0.001) return 0.0; // completly shadowed
		res = min(res, k * h / t);
		t += clamp(h, 0.01, 0.2); // adaptive step to avoid skipping tight corners
	}
	return clamp(res, 0.0, 1.0);
}
// Variables et fonctions retirées :
// - palette() (couleur orbit trap)
// - vec4 trap (calcul orbit trap dans mapScene)

void main()
{
	vec4 finalColor = vec4(0.0);
	
	vec3 ray = get_first_ray_direction();
	s_res result = is_Intersect(ray, cam_pos);	
	
	vec4 baseAmbiant = vec4(1.0, 1.0, 1.0, 1.0) * 0.1;
	vec4 bgColor = vec4(0.0, 0.0, 0.4, 1.0); // Couleur de fond (ciel/fog)
	
	if (result.d <=  0.01)
	{
		vec3    light_pos = cam_pos;
		vec3    to_light  =  normalize( light_pos - result.point);
		
		vec3 n = getNormal(result.point);
		
		// 1. Ambient Occlusion appliqué à l'ambiant base
		float ao = calcAO(result.point, n);
		vec4 ambiant = baseAmbiant * ao;
		
		// 2. Soft Shadow (ombre douce) 
		float distToLight = length(light_pos - result.point);
		// k = 8.0 contrôle la douceur de l'ombre
		float shadow = calcSoftShadow(result.point + n * 0.02, to_light, 0.05, distToLight, 8.0);
		
		// Directional Light/Flashlight sans limite d'angle (lumière globale depuis la caméra)
		float light0 = max(0.0, dot( n, to_light));
		
		// Calcul de la réflexion spéculaire (brillance)
		vec3 view_dir = normalize(cam_pos - result.point);
		vec3 reflect_dir = reflect(-to_light, n);
		// 32.0 est la "brillance" (plus élevé = reflet plus petit et net)
		float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
		// On atténue un peu l'intensité spéculaire (ex: 0.5)
		float specular = 0.5 * spec;
		
		// Couleur de la lumière légèrement jaune
		vec4 lightColor = vec4(1.0, 0.95, 0.8, 1.0);
		
		// Multiplicateur Shadow sur la lumière directe + spéculaire + ambiant
		vec4 directDiffuse = lightColor * (convertRange(to_light, 2.0));
		vec4 color = (directDiffuse * light0 + vec4(specular)) * shadow + ambiant;
		
		// 3. Brouillard de profondeur (Fog)
		// Plus on est loin, plus le fogAmount s'approche de 1.0
		float fogDistance = length(cam_pos - result.point);
		// Paramètres : 10.0 (début du fog), 100.0 (fin du fog)
		float fogAmount = smoothstep(10.0, 100.0, fogDistance);
		
		// Mix final entre la couleur calculée et la couleur de fond
		finalColor = mix(color, bgColor, fogAmount);
	}
	else
	{
		finalColor = bgColor;
	}
	
	// 4. Ajout du Glow (Halo lumineux magique) par dessus l'image finale
	vec4 glowColor = vec4(1.0, 0.8, 0.2, 1.0); // Glow orange/jaune
	finalColor += (result.glow * 0.02) * glowColor;
	
	// --- POST PROCESSING ---
	vec2 center_uv = gl_FragCoord.xy / ScreenResolution;
	
	// 5. Aberration Chromatique
	vec2 fromCenter = center_uv - 0.5;
	float distFromCenter = length(fromCenter);
	
	float aberrationAmount = 0.02 * distFromCenter;
	finalColor.r *= (1.0 + aberrationAmount);
	finalColor.b *= (1.0 - aberrationAmount);
	
	// 6. Vignettage
	float vignette = smoothstep(0.8, 0.2, distFromCenter);
	finalColor *= vignette;
	
	FragColor = finalColor;
}