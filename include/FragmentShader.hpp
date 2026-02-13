#ifndef FRAGMENTSHADER_HPP
# define FRAGMENTSHADER_HPP

# include <iostream> // std::cout std::cerr std::endl
# include <sstream> // std::stringstream
# include <fstream> // std::ifstream
# include <string> // std::string
# include <glad.h> // gl_X

# include "IShader.hpp"

class FragmentShader : public IShader
{
	private:

		std::string 	_file;
		GLuint			_id;

		char			_pad[4];

	public:
	
		FragmentShader();
		FragmentShader( std::string path );
		~FragmentShader();
		bool	createShader(std::string path);
		bool	compileShader( void ) const;	
		const GLuint &getId( void ) const;
		void	deleteShader ( void ) const;

};
#endif