// #include "FCyXt.hpp"
// #include "Window.hpp"


// const char *vertexShaderSource = "#version 330 core\n"
// 	"layout (location = 0) in vec3 aPos;\n"
//     "void main()\n"
//     "{\n"
//     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
//     "}\0";

// const char *fragmentShaderSource = "#version 330 core\n"
//     "out vec4 FragColor;\n"
//     "void main()\n"
//     "{\n"
//     "   FragColor = vec4(1.0f,0.0f,0.0f,1.0f);\n"
//     "}\0";



// int	main(void)
// {
// 	Window	win;
// 	GLuint	VBO;
// 	GLuint	vertexShader;
// 	GLuint	fragmentShader;
// 	GLuint	program;
// 	int	success;
// 	char infoLog[512];

// 	// Prendre la data / Formater le VBO / Ammene sur le GPU 
 	// float	vertices[] = {
 	// 	1.0f,  1.0f, 0.0f,  // top right
 	// 	1.0f, -1.0f, 0.0f,  // bottom right
 	//    -1.0f, -1.0f, 0.0f,  // bottom left
 	//    -1.0f,  1.0f, 0.0f   // top left 
 	// };


// 	glGenBuffers(1, &VBO);														// Generate 1 buffer object -> assigne un ID a VBO
// 	glBindBuffer(GL_ARRAY_BUFFER, VBO); 										// Monte VBO comme le buffer courant
// 	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);	// Colle la data de vertice dans le buffer GPU

// 	// Creer le VextexShader / Initialiser / Compiler

// 	vertexShader = glCreateShader(GL_VERTEX_SHADER);							// Instancie vertex Shader
// 	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);					// Initialise vertexShader
// 	glCompileShader(vertexShader);												// OpenGl compile le shader
// 	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
// 	if(!success)
// 	{
// 	    glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
// 	    std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
// 	}

// 	// Creer le FragmentShader / Initialiser / Compiler

// 	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
// 	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
// 	glCompileShader(fragmentShader);
// 	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
// 	if(!success)
// 	{
// 	    glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
// 	    std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
// 	}

// 	program = glCreateProgram();												// Instancie un programme
// 	glAttachShader(program, vertexShader);
// 	glAttachShader(program, fragmentShader);
// 	glLinkProgram(program);
// 	glGetProgramiv(program, GL_LINK_STATUS, &success);
// 	if (!success)
// 	{
// 	    glGetShaderInfoLog(program, 512, NULL, infoLog);
// 	    std::cerr << "ERROR::LINKAGE\n" << infoLog << std::endl;
// 	}
// 	glDeleteShader(vertexShader);
// 	glDeleteShader(fragmentShader);

// 	glVertexAttribPointer(0,3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
// 	glEnableVertexAttribArray(0);
// 	glUseProgram(program);
// 	return (0); 
// }

#include "FCyXt.hpp"
#include "Window.hpp"
#include <iostream>

// --- SHADERS (Inchangés) ---
const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n" // L'attribut 0 attend 3 floats
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}\0";

int main(void)
{
    Window win; // Initialisation du contexte OpenGL via SDL

    // 1. Définition des données brutes (RAM CPU)
 	float	vertices[] = {
 		1.0f,  1.0f, 0.0f,  // top right
 		1.0f, -1.0f, 0.0f,  // bottom right
 	   -1.0f, -1.0f, 0.0f,  // bottom left
 	   -1.0f,  1.0f, 0.0f,   // top left 
 	   -1.0f, -1.0f, 0.0f,  // bottom left
 		1.0f,  1.0f, 0.0f,  // top right
 	};

    GLuint VBO, VAO; // IDs (noms) pour nos objets GPU
    GLuint program, vertexShader, fragmentShader;
    int success;
    char infoLog[512];

    // ============================================================
    // PHASE 1 : INITIALISATION DES OBJETS (Le "Câblage")
    // ============================================================

    // A. Génération des noms (IDs)
    // Rôle : Réserve des identifiants uniques dans le contexte OpenGL.
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // B. Liaison du VAO (Le Chef d'orchestre)
    // Rôle : Tout appel de configuration futur sera enregistré dans CE VAO.
    glBindVertexArray(VAO);

    // C. Liaison du VBO (Le Conteneur)
    // Rôle : Définit ce VBO comme la cible actuelle pour les opérations sur les tableaux.
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // D. Transfert de données (CPU -> GPU)
    // Rôle : Alloue la mémoire GPU et copie les bits depuis la RAM.
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // E. Configuration de l'Attribut 0 (Le Pont)
    // Rôle : Explique au VAO comment lire le VBO lié pour l'attribut 0.
    // C'est ICI que le lien VAO <-> VBO est scellé.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    // F. Activation de l'Attribut
    // Rôle : Active le "slot" 0 dans le VAO.
    glEnableVertexAttribArray(0);

    // G. Déliason (Optionnel mais propre)
    // On ferme le carnet (VAO) pour ne pas le modifier par erreur ailleurs.
    // Note : On peut délier le VBO *après* le VAO, mais le VAO a déjà mémorisé le lien.
    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0); 

    // ============================================================
    // PHASE 2 : COMPILATION SHADERS (Résumé)
    // ============================================================
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    
    program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // ============================================================
    // PHASE 3 : BOUCLE DE RENDU (Game Loop)
    // ============================================================
    bool running = true;
    SDL_Event e;

    while (running)
    {
        while (SDL_PollEvent(&e)) { if (e.type == SDL_QUIT) running = false; }

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // 1. On active le programme shader
       glUseProgram(program);

        // 2. On réactive LE SEUL objet qui contient toute la config : le VAO
       glBindVertexArray(VAO);

        // 3. On dessine
        // OpenGL lit le VAO -> trouve le VBO associé -> envoie les sommets au shader
        glDrawArrays(GL_TRIANGLES, 0, 6);

        SDL_GL_SwapWindow(win.getWinPtr());
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(program);

    return 0;
}