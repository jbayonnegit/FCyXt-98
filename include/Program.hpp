#pragma once
#include <glad/glad.h>

// Forward declarations
class VertexShader;
class FragmentShader;

class Program {

	private :

		GLuint _id;

	public :

		Program();
		~Program();
		bool	attachLinkShader( VertexShader &vertex, FragmentShader &fragment );
		GLuint	&getId( void );
		void	use( void );
};