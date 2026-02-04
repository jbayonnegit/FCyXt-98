#pragma once
#include "FCyXt.hpp"
#include "IShader.hpp"

class FragmentShader : public IShader
{
	private:

		std::string 	_file;
		GLuint			_id;

	public:
	
		FragmentShader();
		FragmentShader( std::string path );
		~FragmentShader();
		bool	createShader(std::string path);
		bool	compileShader( void ) const;	
		const GLuint &getId( void ) const;
		void	deleteShader ( void ) const;

};