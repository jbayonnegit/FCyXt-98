#ifndef FRACTAL_HPP
# define FRACTAL_HPP

# include <SDL2/SDL.h> // SDL_X
# include <glad.h> // gl_X

# include "Window.hpp"

class Fractal {

    private :

        GLuint VBO;
        GLuint VAO;

    public:

        Fractal();
        ~Fractal();
        void    makeMesh( void );
        void    drawShape( Window &win );

};

#endif