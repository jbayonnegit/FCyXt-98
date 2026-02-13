#ifndef VERTEXSHADER_HPP
# define VERTEXSHADER_HPP

# include <iostream> // std::cout std::cerr std::endl
# include <sstream> // std::stringstream
# include <fstream> // std::ifstream
# include <string> // std::string
# include <glad.h> // gl_X

# include "IShader.hpp"

class VertexShader : public IShader
{
	private:

		std::string 	_file;
		GLuint			_id;

	public:
	
		VertexShader();
		VertexShader( std::string path );
		~VertexShader();
		bool	createShader(std::string path);
		bool	compileShader( void ) const;
		const GLuint &getId( void ) const;
		void	deleteShader ( void ) const;
};

#endif