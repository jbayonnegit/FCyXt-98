#include "Program.hpp"
#include "VertexShader.hpp"
#include "FragmentShader.hpp"
#include <iostream>
#include <cstddef>

Program::Program( void )
{
	_id = glCreateProgram();
}

Program::~Program(){}

bool    Program::attachLinkShader( VertexShader &vertex, FragmentShader &fragment )
{
	int	success;
	char infoLog[512];

	glAttachShader( _id, vertex.getId() );
	glAttachShader( _id, fragment.getId() );
	glLinkProgram( _id );
	glGetProgramiv( _id, GL_LINK_STATUS, &success );
	if ( !success )
	{
		glGetShaderInfoLog( _id, 512, NULL, infoLog );
		std::cerr << "ERROR::LINKAGE\n" << infoLog << std::endl;
		return ( false );
	}
	return ( true );
}

void	Program::use( void )
{
	glUseProgram( _id );
}

GLuint	&Program::getId( void )
{
	return ( _id );
}