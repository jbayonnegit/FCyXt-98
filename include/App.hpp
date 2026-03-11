#ifndef APP_HPP
# define APP_HPP

# include <string> // std::string
# include <glad.h> // gl_X
# include <SDL2/SDL.h> // SDL_X
# include <chrono> // high_resolution_clock

# include "Fractal.hpp"
# include "Window.hpp"
# include "Program.hpp"
# include "Camera.hpp"

class App
{
	private :

		Fractal			_fractal;
		std::string		_name;
		Window			_win;
		Program         _program;
		
		// Smooth zoom animation variables pour mandelbrot
		bool			_isZooming;
		double			_zoomElapsedTime;
		double			_zoomDuration; // 1 second
		double			_zoomStart;
		double			_zoomTarget;
		double			_offsetXStart;
		double			_offsetXTarget;
		double			_offsetYStart;
		double			_offsetYTarget;
		
		bool			runMandelbrot();
		bool			runCircle();


	public :

		App();
		App( const std::string app );
		~App();

		bool    run( void );
};

#endif
