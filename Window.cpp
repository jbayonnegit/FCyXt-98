#include "Window.hpp"

Window::Window()
{ 
	window_loading(); 
}

Window::~Window()
{ 
	SDL_GL_DeleteContext( context );
	SDL_DestroyWindow( window );
}

void	Window::window_loading( void )
{
	if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) < 0)
	{
		fprintf(stderr, "SDL INIT ERROR : %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	window = SDL_CreateWindow("FCyXt-98", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
	if (!window)
	{
		fprintf(stderr, "SDL WINDOW ERROR : %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}
	context = SDL_GL_CreateContext(window);
	if (!context)
	{
		fprintf(stderr, "SDL CONTEXT ERROR  : %s", SDL_GetError());
		exit(EXIT_FAILURE);
	}
}

SDL_Window*	Window::getWinPtr(void)
{
	return (window);
}