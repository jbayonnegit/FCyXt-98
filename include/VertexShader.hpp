#pragma once 
#include "FCyXt.hpp"
#include "IShader.hpp"

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