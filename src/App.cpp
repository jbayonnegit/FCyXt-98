#include "App.hpp"


App::App()
{

}

App::App( const std::string name) : _name ( name )
{
	_fractal.makeMesh();
	_program.runProgram( _name );
}



bool	App::run( void )
{
	const unsigned char* version = glGetString(GL_VERSION);
    const unsigned char* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);
    const unsigned char* vendor = glGetString(GL_VENDOR);
    const unsigned char* renderer = glGetString(GL_RENDERER);
    std::cout << "--- Infos GPU ---" << std::endl;
    std::cout << "Carte : " << renderer << std::endl;
    std::cout << "Fabricant : " << vendor << std::endl;
    std::cout << "Version OpenGL : " << version << std::endl;
    std::cout << "Version GLSL : " << glslVersion << std::endl;
    std::cout << "-----------------" << std::endl;

	if ( "Mandelbrot" == _name )
		runMandelbrot();
	else if ( "Circle" == _name )
		runCircle();
	return true;
}

bool	App::runMandelbrot( void )
{
	bool running = true;
	SDL_Event		e;
	double z;
	z = 1;
	double			osX = 0;
	int			f = 1;
	double			osY = 0;
	double			c = 0;
	while ( running )
	{
		GLuint u_resolution  = glGetUniformLocation( _program.getId(), "u_resolution");
		GLuint zoom  = glGetUniformLocation( _program.getId(), "zoom");
		GLuint offsetX  = glGetUniformLocation( _program.getId(), "offsetX");
		GLuint offsetY  = glGetUniformLocation( _program.getId(), "offsetY");
		GLuint limit  = glGetUniformLocation( _program.getId(), "limit");
		GLuint cr  = glGetUniformLocation( _program.getId(), "cr");
		glUniform2d(u_resolution, WIDTH, HEIGHT);
		while ( SDL_PollEvent(&e) )
		{
			if ( SDL_QUIT == e.type )
				running = false;
			if (e.type == SDL_MOUSEWHEEL)
			{
				int mouseX, mouseY;
				
				SDL_GetMouseState( &mouseX, &mouseY );
				
				mouseY -= HEIGHT;
				mouseY *= -1;

				double mouseWorldXBefore = osX + (mouseX * z);
   				double mouseWorldYBefore = osY + (mouseY * z);
				if (e.wheel.y < 0)
					z *= 1.05;
				else if (e.wheel.y > 0)
					z *= 0.95;
				osX = mouseWorldXBefore - (mouseX * z);
  				osY = mouseWorldYBefore - (mouseY * z);
			}
			if ( e.type == SDL_KEYDOWN )
			{
				if ( e.key.keysym.sym == 0x006d)
				{
					f += 10;
				}	
			}
		}
		c += 0.0001;
		glUniform1i( limit, f );
		glUniform1d( zoom, z );
		glUniform1d( cr, c );
		glUniform1d( offsetX, osX );
		glUniform1d( offsetY, osY );
		_fractal.drawShape( _win );
	//	SDL_Delay( 20 );
 	}
	return true;
}

bool	App::runCircle(void)
{
	bool running = true;
	SDL_Event		e;
	Vec3		sphere(-30, 0, 0);
	Vec3		cube(-5, 7, -10);
	double		i = 1;
	double		ratio = 1;
	double		angle = 0;
	Camera	camera(1.45,-30,0);

	auto last = std::chrono::high_resolution_clock::now();
    int frameCount = 0;
    double fpsTimer = 0.0;
    double currentFPS = 0.0;

	GLuint cam_pos	= glGetUniformLocation( _program.getId(), "cam_pos" );
	GLuint spherePos	= glGetUniformLocation( _program.getId(), "spherePos" );
	GLuint cubePos	= glGetUniformLocation( _program.getId(), "cubePos" );
	GLuint cam_dir	= glGetUniformLocation( _program.getId(), "forward" );
	GLuint right	= glGetUniformLocation( _program.getId(), "right" );
	GLuint up		= glGetUniformLocation( _program.getId(), "up" );
	GLuint ViewportResolution	= glGetUniformLocation( _program.getId(), "resolution" );
	GLuint ScreenResolution	= glGetUniformLocation( _program.getId(), "ScreenResolution" );

	while (running)
	{
		auto now = std::chrono::high_resolution_clock::now();
        double dt = std::chrono::duration<double>(now - last).count();
        last = now;

        // FPS counter
        frameCount++;
        fpsTimer += dt;
        if (fpsTimer >= 1.0)
        {
            currentFPS = frameCount / fpsTimer;
            std::cout << "\r[FPS: " << (int)currentFPS << "]          " << std::flush;
            frameCount = 0;
            fpsTimer = 0.0;
        }
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		cube._x = 15.0 * cos(angle * 0.5 );
		cube._y = 15.0 * sin(angle * 0.5 );
		cube._z += i * 0.5;
		if ( cube._z <= -10)
			i = 1;
		else if ( cube._z >= 10 )
			i = -1;
		sphere._x = 10.0 * cos(angle);
		sphere._y = 10.0 * sin(angle);
		angle += 0.07;
		
		while ( SDL_PollEvent(&e) )
		{
			if ( SDL_QUIT == e.type )
				running = false;
			if ( e.type == SDL_MOUSEWHEEL )
			{
				if ( e.wheel.y < 0 )
					camera.moveBackward();
				else if ( e.wheel.y > 0 )
					camera.moveForward();
			
			}
			if ( e.type == SDL_KEYUP)
			{
				if (e.key.keysym.sym == 1073742049)
					camera.shiftIsPress = false; 
			}
			if ( e.type == SDL_KEYDOWN)
			{
				if (e.key.keysym.sym == 1073742049)
					camera.shiftIsPress = true;
			}
			if ( e.type == SDL_MOUSEBUTTONDOWN )
			{
				if (e.button.button == 2)
					camera.handleMouseMidButtonPress( e.button.x, e.button.y );
			}
			else if ( e.type == SDL_MOUSEBUTTONUP )
			{
				if (e.button.button == 2)
					camera.midIsPress = false;
			}

			if ( e.type == SDL_MOUSEMOTION )
			{
				camera.handleMouseMovement( e.button.x, e.button.y );
			}
		}

		
		glUniform3f( cam_pos, camera.position._x , camera.position._y , camera.position._z );
		glUniform3f( spherePos, sphere._x , sphere._y , sphere._z );
		glUniform3f( cubePos, cube._x , cube._y , cube._z );
		glUniform3f( cam_dir, camera.front._x , camera.front._y , camera.front._z );
		glUniform3f( up , camera.up._x, camera.up._y , camera.up._z);
		glUniform3f( right , camera.right._x, camera.right._y , camera.right._z );
		glUniform2f( ViewportResolution, camera.w, camera.h);
		glUniform2f( ScreenResolution, WIDTH, HEIGHT);

		_fractal.drawShape( _win );

	}
	return ( true );
}

App::~App()
{
	
}