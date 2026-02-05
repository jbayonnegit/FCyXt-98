 #version 410 core

	out vec4 FragColor;
	uniform vec2 u_resolution;
	uniform float zoom;
	uniform int limit;
	uniform float offsetX;
	uniform float offsetY;

	struct    complex{

		float r;
		float i;
	};


	float    convert_range(float v, float max_src, float min_dst, float max_dst)
	{
		return (min_dst + v * (max_dst - min_dst) / max_src);
	}

	vec3   get_color(int it)
	{
		vec3	color;
		float    t;

		if (it == limit)
			return (vec3(0x000000));
		t = float(it) / limit;
		color.x = (9 * (1 - t) * t * t * t);
		color.y = (15 * (1 - t) * (1 - t) * t * t);
		color.z = (8.5 * (1 - t) * (1 - t) * (1 - t) * t);
		return (color);
	}

	void    display_pixel(float x, float y, int it)
	{
		vec3 color;
		float u = x;
		float k = y;

		u += 0.5f;
		k += 0.5f;
		color = get_color( it );
		if ((gl_FragCoord.x >= (u - 0.5f) && gl_FragCoord.x <= (u + 0.5f))
		&& (gl_FragCoord.y >= (k - 0.5f) && gl_FragCoord.y <= (k + 0.5f)))
			FragColor = vec4(
				color.x,  
				color.y, 
				color.z, 
				1.0f);
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
		result.r = pow(c.r, 2) - pow(c.i, 2);
		result.i = 2.0f * c.r * c.i;
		return result;
	}


	void main()
	{
		float x = convert_range(gl_FragCoord.x * zoom + offsetX, u_resolution.x, -2 * 1.77777777778, 2 * 1.77777777778);
		float y = convert_range(gl_FragCoord.y * zoom + offsetY, u_resolution.y, -2, 2);
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
			if ( (z.r * z.r) + (z.i * z.i) > 4.0f )
			{
				display_pixel(gl_FragCoord.x , gl_FragCoord.y , it);
				return ;
			}
			it += 1;            
		}
		FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	} 