#include "Fractal.hpp"
#include "Window.hpp"
#include <SDL2/SDL.h>

Fractal::Fractal(){}

Fractal::~Fractal(){}

void    Fractal::makeMesh( void )
{

    float	vertices[] = {
 		1.0f,  1.0f, 0.0f,  // top right
 		1.0f, -1.0f, 0.0f,  // bottom right
 	   -1.0f, -1.0f, 0.0f,  // bottom left
 	   -1.0f,  1.0f, 0.0f,   // top left 
 	   -1.0f, -1.0f, 0.0f,  // bottom left
 		1.0f,  1.0f, 0.0f,  // top right
 	};
    glGenVertexArrays(1, &VAO);                                                 // Ouvre "l'enregistrement" du VAO
	glGenBuffers(1, &VBO);														// Generate 1 buffer object -> assigne un ID a VBO
	glBindVertexArray( VAO );
    glBindBuffer(GL_ARRAY_BUFFER, VBO); 										// Monte VBO comme le buffer courant
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// Colle la data de vertice dans le buffer GPU
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
}

void    Fractal::drawShape( Window &win )
{
    glBindVertexArray( VAO );
	glDrawArrays( GL_TRIANGLES, 0, 6 );
	SDL_GL_SwapWindow( win.getWinPtr() );
}