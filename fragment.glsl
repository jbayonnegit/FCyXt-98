#version 330 core
	out vec4 FragColor;
	uniform vec2 u_resolution;
	uniform float zoom;
	uniform float offsetX;
	uniform float offsetY;

	struct	complex{

		float r;
		float i;
	};

	float	convert_range(float v, float max_src, float min_dst, float max_dst)
	{
		return (min_dst + v * (max_dst - min_dst) / max_src);
	}

	void	display_pixel(float x, float y, float limit)
	{
		float u = x;
		float k = y;
		u += 0.5f;
		k += 0.5f;
		if ((gl_FragCoord.x >= (u - 0.5f) && gl_FragCoord.x <= (u + 0.5f))
		&& (gl_FragCoord.y >= (k - 0.5f) && gl_FragCoord.y <= (k + 0.5f)))
			FragColor = vec4(limit, 0.0f ,0.0f,1.0f);
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
		result.i = 2 * c.r * c.i;
		return result;
	}

	void main()
	{
		float x = convert_range((gl_FragCoord.x + offsetX) * zoom , u_resolution.x, -2 * 1.77777777778, 2 * 1.77777777778);
		float y = convert_range((gl_FragCoord.y + offsetY) * zoom , u_resolution.y, -2, 2);
		complex c;
		complex z;
		float	limit = 0;
		c.r = 0.4;
		c.i = 0.35;
		z.r = x;
		z.i = y;
		while ( limit < 200 )
		{
			z = add_complex(sqr_complex( z ), c);
			if ( (z.r * z.r) + (z.i * z.i) > 4 )
			{
				display_pixel(gl_FragCoord.x, gl_FragCoord.y, convert_range(limit, 20, 0, 1));
				return ;
			}
			limit += 1;			
		}
		FragColor = vec4(0.0f, 0.0f ,0.0f,1.0f);
	}
