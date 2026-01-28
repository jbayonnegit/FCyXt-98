#pragma once
#include "FCyXt.hpp"
#include <string>

class Shader
{
	private:

		GLuint	ID;

	public:
	
		Shader( std::string path );
		Shader();
		~Shader();
};
