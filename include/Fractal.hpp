#pragma once
#include <glad/glad.h>

// Forward declaration
class Window;

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