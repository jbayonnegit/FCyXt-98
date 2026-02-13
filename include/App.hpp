#ifndef APP_HPP
# define APP_HPP

# include <string> // std::string
# include <glad.h> // gl_X
# include <SDL2/SDL.h> // SDL_X

# include "Fractal.hpp"
# include "Window.hpp"
# include "Program.hpp"

class App
{
	private :

		Fractal			_fractal;
		std::string		_name;
		Window			_win;
		Program         _program;
		bool			runMandelbrot();


	public :

		App();
		App( const std::string app );
		~App();

		bool    run( void );
};

#endif
