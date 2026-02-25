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
  dvec3 q = abs(p) - b;
  return length(max(q,0.0)) + min(max(q.x,max(q.y,q.z)),0.0);
}

void main ()
{
	dvec3 	ray;
	double	d;

	dvec3 point = cam_pos;

	ray = get_first_ray_direction();
	d = sdBox(point, dvec3(1,1,1));
	while (d > 0.01)
	{
		d = sdBox(point, dvec3(1,1,1));
		point += d * ray;
		if ( d > 500 )
			break ;
	}
	if (d <= 0.01)
		FragColor = vec4(1.0, 0, 0, 1);
	else
		FragColor = vec4(0, 0.2, 0, 1);
	//display_pixel(100, 100);
}