#include "FCyXt.hpp"
#include "Window.hpp"

int	main(void)
{
	Window	win;
	GLuint	VBO;
	GLuint	VAO;
	GLuint	vertexShader;
	GLuint	fragmentShader;
	GLuint	program;
	int	success;
	char infoLog[512];

	// Prendre la data / Formater le VBO / Ammene sur le GPU 
 	float	vertices[] = {
 		1.0f,  1.0f, 0.0f,  // top right
 		1.0f, -1.0f, 0.0f,  // bottom right
 	   -1.0f, -1.0f, 0.0f,  // bottom left
 	   -1.0f,  1.0f, 0.0f,   // top left 
 	   -1.0f, -1.0f, 0.0f,  // bottom left
 		1.0f,  1.0f, 0.0f,  // top right
 	};

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);														// Generate 1 buffer object -> assigne un ID a VBO
	glBindVertexArray( VAO );
	glBindBuffer(GL_ARRAY_BUFFER, VBO); 										// Monte VBO comme le buffer courant
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// Colle la data de vertice dans le buffer GPU
	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);
	// Creer le VextexShader / Initialiser / Compiler

	vertexShader = glCreateShader(GL_VERTEX_SHADER);							// Instancie vertex Shader
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);					// Initialise vertexShader
	glCompileShader(vertexShader);												// OpenGl compile le shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
	    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
	    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// Creer le FragmentShader / Initialiser / Compiler

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	if(!success)
	{
	    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
	    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	program = glCreateProgram();												// Instancie un programme
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success)
	{
	    glGetShaderInfoLog(program, 512, NULL, infoLog);
	    std::cerr << "ERROR::LINKAGE\n" << infoLog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	glUseProgram(program);

	bool running = true;
	SDL_Event	e;

	while (running)
	{
		while ( SDL_PollEvent(&e) )
		{
			if ( SDL_QUIT == e.type )
				running = false;
		}
		glBindVertexArray( VAO );
		glDrawArrays( GL_TRIANGLES, 0, 6 );

		SDL_GL_SwapWindow( win.getWinPtr() );
		SDL_Delay( 100 );
	}
	return (0); 
}
