#include "FCyXt.hpp"
#include <cmath>


int	main(void)
{
	Window			win;
	VertexShader	vertexShader("vertex.glsl");
	FragmentShader	fragmentShader("fragment.glsl");
	Fractal			fractal;
	Program			program;
	bool running = true;
	SDL_Event		e;

	fractal.makeMesh();
	vertexShader.compileShader();
	fragmentShader.compileShader();
	program.attachLinkShader( vertexShader, fragmentShader );
	vertexShader.deleteShader();
	fragmentShader.deleteShader();
	program.use();

	while ( running )
	{
		//float timeValue = SDL_GetTicks() / 1000.0f;
		//float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
		GLuint u_resolution  = glGetUniformLocation( program.getId(), "u_resolution");
		glUniform2f(u_resolution, WIDTH, HEIGHT);
		while ( SDL_PollEvent(&e) )
		{
			if ( SDL_QUIT == e.type )
				running = false;
		}
		fractal.drawShape( win );
 	}

	return (0);
}
