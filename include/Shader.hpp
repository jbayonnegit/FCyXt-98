#pragma once
#include <string>
class Shader
{
	private:

		std::string	ShaderFragment;
		std::string	ShaderVertex;

	public:
	
		Shader(std::string fragmentPath, std::string vertexPath);
		Shader();
		~Shader();
};
