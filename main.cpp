#include "FCyXt.hpp"
#include "Window.hpp"
#

int	main(void)
{
	Window	win;

	        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Dessin du carré
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glColor3f(1.0f, 0.0f, 0.0f); // rouge
        glBegin(GL_POINTS);
            glVertex2f(0.0f,  0.0f);
        glEnd();
        SDL_GL_SwapWindow(win.getWinPtr());
	SDL_Delay(2000);
	return (0);
}