#ifndef PROGRAM_HPP
# define PROGRAM_HPP

# include <string> // std::string
# include <glad.h> // gl_X

#include "VertexShader.hpp"
#include "FragmentShader.hpp"

class Program {

	private :

		VertexShader	*_vertexShader;
	    FragmentShader	*_mandelbrot;
		GLuint 			_id;

		char			_pad[4];

	public :

		Program();
		~Program();
		bool	attachLinkShader( VertexShader &vertex, FragmentShader &fragment );
		bool	runProgram( const std::string name );
		GLuint	&getId( void );
		void	use( void );

};

#endif