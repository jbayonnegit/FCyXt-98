#include "VertexShader.hpp" 

VertexShader::VertexShader(){}

VertexShader::VertexShader( std::string path )
{
	id = createShader( path );
}

VertexShader::~VertexShader(){}

const   GLuint  &VertexShader::getId( void ) const 
{
	return ( id );
}

void	VertexShader::deleteShader( void ) const
{
	glDeleteShader( id );
}

bool	VertexShader::compileShader( void ) const
{
	char	infoLog[512];
	int		success;

	glCompileShader(id);
	glGetShaderiv(id, GL_COMPILE_STATUS, &success);
	if(!success)
	{
	    glGetShaderInfoLog(id, 512, NULL, infoLog);
	    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return ( false );
	}
	return ( true );
}

bool	VertexShader::createShader(std::string path)
{
	std::stringstream	buffer;
	std::string			finalString;
	std::ifstream		file;

	file.open( path.c_str() );
	if (!file.good())
		std::cout << "File failed" << std::endl;
	buffer << file.rdbuf();
	std::cout << buffer.str() << std::endl;
	file.close();
	return 0;
}
