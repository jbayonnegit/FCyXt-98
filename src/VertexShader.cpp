#include "VertexShader.hpp" 

VertexShader::VertexShader(){}

VertexShader::VertexShader( std::string path )
{
	if ( !createShader( path ) )
		std::cerr << "Failed in vertex shader constructor " << std::endl;
}

VertexShader::~VertexShader(){}

const   GLuint  &VertexShader::getId( void ) const 
{
	return ( _id );
}

void	VertexShader::deleteShader( void ) const
{
	glDeleteShader( _id );
}

bool	VertexShader::compileShader( void ) const
{
	char	infoLog[512];
	int		success;

	glCompileShader(_id);
	glGetShaderiv(_id, GL_COMPILE_STATUS, &success);
	if(!success)
	{
	    glGetShaderInfoLog(_id, 512, NULL, infoLog);
	    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return ( false );
	}
	return ( true );
}

bool	VertexShader::createShader(std::string path)
{
	std::stringstream		buffer;
	std::ifstream			file;
	const char				*str;

	file.open( path.c_str() );
	if (!file.good())
	{
		std::cout << "File failedd" << std::endl;
		return ( false );
	}
	buffer << file.rdbuf();
	file.close();
	_file = buffer.str();
	str = _file.c_str();
	_id = glCreateShader( GL_VERTEX_SHADER );
	glShaderSource( _id, 1, &str, NULL);
	return true;
}
