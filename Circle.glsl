#version 410 core
#extension GL_ARB_gpu_shader_fp64 : require

out vec4 FragColor;
uniform dvec3	cam_pos;	
uniform dvec3	forward;
uniform dvec3	up;
uniform dvec3	right;
uniform dvec2	resolution;
uniform dvec2	ScreenResolution;

dvec3	get_first_ray_direction(void)
{
	dvec3	point_on_displayport 	= cam_pos 
									+ forward 
									+ (-up * (resolution.y / 2)) 
									+ (-right * (resolution.x / 2));

	dvec2 uv = gl_FragCoord.xy / ScreenResolution;

	point_on_displayport += ( up * (uv.y * resolution.y) ) + ( right * (uv.x * resolution.x) );

	dvec3	ray_direction = point_on_displayport - cam_pos;

	return (normalize(ray_direction));
}


void    display_pixel(double x, double y)
{
	dvec3 color;
	double u = x;
	double k = y;

	u += 0.5;
	k += 0.5;
	if ((gl_FragCoord.x >= (u - 0.5) && gl_FragCoord.x <= (u + 0.5))
	&& (gl_FragCoord.y >= (k - 0.5) && gl_FragCoord.y <= (k + 0.5)))
		FragColor = vec4( 1.0f, 0.0f ,0.0f ,1.0f);
}


double	SdSphere(dvec3 p, double r)
{
	double d = length(p) - r;
	return (d);
}

double sdBox( dvec3 p, dvec3 b )
{

	//p = p - dvec3(10.0, 0.0, 0.0);
  dvec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

double sdPyramid( dvec3 p, double h )
{
  double m2 = h*h + 0.25;
    
  p.xz = abs(p.xz);
  p.xz = (p.z>p.x) ? p.zx : p.xz;
  p.xz -= 0.5;

  dvec3 q = dvec3( p.z, h*p.y - 0.5*p.x, h*p.x + 0.5*p.y);
  double s = max(-q.x,0.0);
  double t = clamp( (q.y-0.5*p.z)/(m2+0.25), 0.0, 1.0 );
  double a = m2*(q.x+s)*(q.x+s) + q.y*q.y;
  double b = m2*(q.x+0.5*t)*(q.x+0.5*t) + (q.y-m2*t)*(q.y-m2*t);
    
  double d2 = min(q.y,-q.x*m2-q.y*0.5) > 0.0 ? 0.0 : min(a,b);
  return sqrt( (d2+q.z*q.z)/m2 ) * sign(max(q.z,-p.y));
}

double	mapScene(dvec3 p)
{
	double	sphere = SdSphere(p ,2);
	double	box = sdBox(p, dvec3(1,1,1));

	return (box);
}

dvec3 getNormal(dvec3 p)
{
    const double eps = 0.01;
    return normalize
    (    dvec3
        (    mapScene(p + dvec3(eps, 0, 0)) - mapScene(p - dvec3(eps, 0, 0)),
            mapScene(p + dvec3(0, eps, 0)) - mapScene(p - dvec3(0, eps, 0)),
            mapScene(p + dvec3(0, 0, eps)) - mapScene(p - dvec3(0, 0, eps))
        )
    );
}

void main ()
{
	dvec3 	ray;
	double	d;
	double	normal;
	double	light_angle;
	dvec3	n;
	dvec3	point = cam_pos;

	ray = get_first_ray_direction();
	d = mapScene(point);
	while (d > 0.1)
	{
		d = mapScene(point);
		point += d * ray;
		if ( d > 1000 )
			break ;
	}
	n = getNormal(point);
	light_angle = dot( n, normalize( point - dvec3(10, 10, 10) ));
	if (d <= 0.1)
		FragColor = vec4(1.0 * light_angle, 0, 0, 1);
	else
		FragColor = vec4(0, 0.2, 0, 1);
}