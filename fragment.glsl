#version 410 core
#extension GL_ARB_gpu_shader_fp64 : require

	out vec4 FragColor;
	uniform dvec2 u_resolution;
	uniform double zoom;
	uniform int limit;
	uniform double offsetX;
	uniform double offsetY;

	struct    complex{

		double r;
		double i;
	};


	double    convert_range(double v, double max_src, double min_dst, double max_dst)
	{
		return (min_dst + v * (max_dst - min_dst) / max_src);
	}

	dvec3   get_color(int it)
	{
		dvec3	color;
		double    t;

		if (it == limit)
			return (dvec3(0x000000));
		t = double(it) / limit;
		color.x = (9 * (1 - t) * t * t * t);
		color.y = (15 * (1 - t) * (1 - t) * t * t);
		color.z = (8.5 * (1 - t) * (1 - t) * (1 - t) * t);
		return (color);
	}

	void    display_pixel(double x, double y, int it)
	{
		dvec3 color;
		double u = x;
		double k = y;

		u += 0.5;
		k += 0.5;
		color = get_color( it );
		if ((gl_FragCoord.x >= (u - 0.5) && gl_FragCoord.x <= (u + 0.5))
		&& (gl_FragCoord.y >= (k - 0.5) && gl_FragCoord.y <= (k + 0.5)))
			FragColor = vec4( 1.0f, 0.0f ,0.0f ,1.0f);
			FragColor = vec4(
			 	color.x,  
			 	color.y, 
			 	color.z, 
			 	1.0);
	}

	complex add_complex(complex c1, complex c2)
	{
		complex result;
		result.r = c1.r + c2.r;
		result.i = c1.i + c2.i;
		return result;
	}

	complex sqr_complex(complex c)
	{
		complex result;
		result.r = ( c.r * c.r ) - ( c.i * c.i );
		result.i = 2.0 * c.r * c.i;
		return result;
	}

	void main()
	{
		double x = convert_range(gl_FragCoord.x * zoom + offsetX, u_resolution.x, -2 * 1.77777777778, 2 * 1.77777777778);
		double y = convert_range(gl_FragCoord.y * zoom + offsetY, u_resolution.y, -2, 2);
		complex c;
		complex z;
		int    it = 0;
		c.r = x;
		c.i = y;
		z.r = x;
		z.i = y;
		while ( it < limit )
		{
			z = add_complex(sqr_complex( z ), c);
			if ( (z.r * z.r) + (z.i * z.i) > 4.0 )
			{
				display_pixel(gl_FragCoord.x , gl_FragCoord.y , it);
				return ;
			}
			it += 1;            
		}
		FragColor = vec4(0.0, 0.0, 0.0, 1.0);
	} 