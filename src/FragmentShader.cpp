#include "FragmentShader.hpp" 

FragmentShader::FragmentShader(){}

FragmentShader::FragmentShader( std::string path )
{
	if ( !createShader( path ) )
		std::cerr << "Failed in fragment shader constructor " << std::endl;
}

FragmentShader::~FragmentShader(){}

const   GLuint  &FragmentShader::getId( void ) const 
{
	return ( _id );
}

void	FragmentShader::deleteShader( void ) const
{
	glDeleteShader( _id );
}

bool	FragmentShader::compileShader( void ) const
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

bool	FragmentShader::createShader(std::string path)
{
	std::stringstream       buffer;
	std::ifstream			file;
	const char				*str;

	file.open( path.c_str() );
	if (!file.good())
	{
		std::cout << "File failed" << std::endl;
		return false;
	}
	buffer << file.rdbuf();
	file.close();
	_file = buffer.str();
	str = _file.c_str();
	_id = glCreateShader( GL_FRAGMENT_SHADER );
	glShaderSource( _id, 1, &str, NULL);
	return true;
}
