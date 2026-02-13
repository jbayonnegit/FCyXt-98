#ifndef ISHADER_HPP 
# define ISHADER_HPP 

# include <string> // std::string

class IShader
{
	public:

		virtual	~IShader(){}
		virtual bool	createShader(std::string path) = 0;
		virtual bool	compileShader( void )	const = 0;
		virtual	const GLuint &getId( void )		const = 0;
		virtual void	deleteShader( void )	const = 0;

};
#endif