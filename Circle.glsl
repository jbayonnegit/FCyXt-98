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

float	mapScene(vec3 p)
{
	float	sphere1 = SdSphere(p ,5, spherePos);
	float	sphere0 = SdSphere(p ,10, vec3(0));
	float	box = sdBox(p, vec3(4,4,4));
	return (opSmoothUnion(opSmoothUnion(sphere0, sphere1, 0.5), box, 2));
}

vec3 getNormal(vec3 p)
{
    const float eps = 0.01;
    return normalize
    (    vec3
        (    mapScene(p + vec3(eps, 0, 0)) - mapScene(p - vec3(eps, 0, 0)),
            mapScene(p + vec3(0, eps, 0)) - mapScene(p - vec3(0, eps, 0)),
            mapScene(p + vec3(0, 0, eps)) - mapScene(p - vec3(0, 0, eps))
        )
    );
}

void main ()
{
	vec3 	ray;
	float	d;
	float	normal;

	vec3	n;
	vec3	point = cam_pos;

	ray = get_first_ray_direction();
	d = mapScene(point);
	while (d > 0.001)
	{
		d = mapScene(point);
		point += d * ray;
		if ( d > 1000 )
			break ;
	}
	n = getNormal(point);
	float light0 = dot( n, normalize(vec3(-300, -20, 300) - point ));
	float light1 = dot( n, normalize(vec3(300, -20, 300) - point ));
	float light2 = dot( n, normalize(vec3(0, -20, -300) - point ));
	if (d <= 1)
	{
		vec4 color = vec4( 1.0 * light0, 0, 0, 1) + vec4(0 , 0,  1 * light1, 1) + vec4(0 , 1 * light2,  0, 1);
		FragColor = color;
	}
	else
		FragColor = vec4(0, 0.002, 0, 1);
}