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

	float z;

	fractal.makeMesh();
	vertexShader.compileShader();
	fragmentShader.compileShader();
	program.attachLinkShader( vertexShader, fragmentShader );
	vertexShader.deleteShader();
	fragmentShader.deleteShader();
	program.use();

	z = 1;

	while ( running )
	{
		

		GLuint u_resolution  = glGetUniformLocation( program.getId(), "u_resolution");
		GLuint zoom  = glGetUniformLocation( program.getId(), "zoom");
		GLuint offsetX  = glGetUniformLocation( program.getId(), "offsetX");
		GLuint offsetY  = glGetUniformLocation( program.getId(), "offsetY");
		glUniform2f(u_resolution, WIDTH, HEIGHT);
		glUniform1f(zoom, z);
		float			osX = 0;
		float			osY = 0;
		while ( SDL_PollEvent(&e) )
		{
			if ( SDL_QUIT == e.type )
				running = false;
			if (e.type == SDL_MOUSEWHEEL)
			{
				int mouseX, mouseY;
				
				SDL_GetMouseState( &mouseX, &mouseY );
				
				mouseY -= HEIGHT;
				mouseY *= -1;
				std::cout << " mouse x: " << mouseX << " mouse Y : " << mouseY << std::endl;

				float mouseWorldXBefore = osX + (mouseX * z);
   				float mouseWorldYBefore = osY + (mouseY * z);
				if (e.wheel.y < 0)
				{
					z *= 1.05; // Zoom in
				}
				else if (e.wheel.y > 0)
				{
					z *= 0.95; // Zoom out
				}
				osX = mouseWorldXBefore - (mouseX * z);
  				osY = mouseWorldYBefore - (mouseY * z);
				std::cout << "  osx: " << osX << " osy : " << osY << std::endl;
			}
		}

		glUniform1f(offsetX, osX);
		glUniform1f(offsetY, osY);
		
		fractal.drawShape( win );
		SDL_Delay(2000);
 	}

	return (0);
}
