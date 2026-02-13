#ifndef WINDOW_HPP
# define WINDOW_HPP

# include <iostream> // std::cout std::cerr std::endl
# include <glad.h> // gl_X
# include <SDL2/SDL.h> // SDL_X

# define	HEIGHT 1080
# define	WIDTH 1920

class Window
{

	public:

		Window();
		~Window();
		SDL_Window	*getWinPtr(void);
		
	private:

		SDL_Window		*window;
		SDL_GLContext	context;
		void			window_loading( void );

};

#endif