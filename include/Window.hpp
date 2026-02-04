#pragma once 
# include "FCyXt.hpp"

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
