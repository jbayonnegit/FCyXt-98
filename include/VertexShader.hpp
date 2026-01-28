#include "FCyXt.hpp"
#include <sstream>
#include "IShader.hpp"

class VertexShader : public IShader
{
	private:

		std::fstream 	file;
		GLuint			id;

	public:
	
		VertexShader();
		VertexShader( std::string path );
		~VertexShader();
		bool	createShader(std::string path);
		bool	compileShader( void ) const;
		const GLuint &getId( void ) const;
		void	deleteShader ( void ) const;
};