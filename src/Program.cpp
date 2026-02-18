#include "Program.hpp"

Program::Program( void )
{
	_vertexShader.createShader("vertex.glsl");
	_mandelbrot.createShader("fragment.glsl");
	_circle.createShader("Circle.glsl");
	_id = glCreateProgram();
}

Program::~Program()
{

}

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

bool	Program::runProgram( const std::string name )
{
	if ( "Mandelbrot" == name )
	{
		_vertexShader.compileShader();
		_mandelbrot.compileShader();
		attachLinkShader( _vertexShader, _mandelbrot );
		_vertexShader.deleteShader();
		_mandelbrot.deleteShader();
	}
	if ( "Circle" == name )
	{
		_vertexShader.compileShader();
		_circle.compileShader();
		attachLinkShader( _vertexShader, _circle );
		_vertexShader.deleteShader();
		_circle.deleteShader();
	}
	use();
	return true;
}

GLuint	&Program::getId( void )
{
	return ( _id );
}