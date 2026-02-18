#version 410 core
#extension GL_ARB_gpu_shader_fp64 : require

out vec4 FragColor;
uniform vec3 cam_pos;	
uniform vec3 cam_direction;
uniform vec3 up;
uniform vec3 right;


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


void main ()
{
	FragColor = vec4(0.0, 0.0, 0.0, 1.0);

	display_pixel(100, 100);
}