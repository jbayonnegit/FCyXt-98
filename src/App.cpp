#include "App.hpp"



App::App()
{

}

App::App( const std::string name) : _name ( name )
{
	_fractal.makeMesh();
	_program.runProgram( _name );
}



bool	App::run( void )
{
	if ( "Mandelbrot" == _name )
		runMandelbrot();
	return true;
}

bool	App::runMandelbrot( void )
{
	bool running = true;
	SDL_Event		e;
	double z;

    const unsigned char* version = glGetString(GL_VERSION);
    const unsigned char* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    const unsigned char* vendor = glGetString(GL_VENDOR);
    const unsigned char* renderer = glGetString(GL_RENDERER);
    std::cout << "--- Infos GPU ---" << std::endl;
    std::cout << "Carte : " << renderer << std::endl;
    std::cout << "Fabricant : " << vendor << std::endl;
    std::cout << "Version OpenGL : " << version << std::endl;
    std::cout << "Version GLSL : " << glslVersion << std::endl;
    std::cout << "-----------------" << std::endl;
	z = 1;
	double			osX = 0;
	int			f = 1;
	double			osY = 0;
	double			c = 0;
	while ( running )
	{
		GLuint u_resolution  = glGetUniformLocation( _program.getId(), "u_resolution");
		GLuint zoom  = glGetUniformLocation( _program.getId(), "zoom");
		GLuint offsetX  = glGetUniformLocation( _program.getId(), "offsetX");
		GLuint offsetY  = glGetUniformLocation( _program.getId(), "offsetY");
		GLuint limit  = glGetUniformLocation( _program.getId(), "limit");
		GLuint cr  = glGetUniformLocation( _program.getId(), "cr");
		glUniform2d(u_resolution, WIDTH, HEIGHT);
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

				double mouseWorldXBefore = osX + (mouseX * z);
   				double mouseWorldYBefore = osY + (mouseY * z);
				if (e.wheel.y < 0)
					z *= 1.05;
				else if (e.wheel.y > 0)
					z *= 0.95;
				osX = mouseWorldXBefore - (mouseX * z);
  				osY = mouseWorldYBefore - (mouseY * z);
			}
			if ( e.type == SDL_KEYDOWN )
			{
				if ( e.key.keysym.sym == 0x006d)
				{
					f += 10;
				}	
			}
		}
		c += 0.0001;
		glUniform1i(limit, f);
		glUniform1d(zoom, z);
		glUniform1d(cr, c);
		glUniform1d(offsetX, osX);
		glUniform1d(offsetY, osY);
		_fractal.drawShape( _win );
		SDL_Delay(20);
 	}
	return true;
}

App::~App()
{
	
}