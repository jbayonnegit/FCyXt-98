#include "App.hpp"
#include <cmath>


App::App()
{

}

App::App( const std::string name) : _name ( name )
{
	_fractal.makeMesh();
	_program.runProgram( _name );
	
	// Initialize zoom animation variables
	_isZooming = false;
	_zoomElapsedTime = 0.0;
	_zoomDuration = 1.0;
	_zoomStart = 1.0;
	_zoomTarget = 1.0;
	_offsetXStart = 0.0;
	_offsetXTarget = 0.0;
	_offsetYStart = 0.0;
	_offsetYTarget = 0.0;
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
	int			f = 100;  // Réduit à 100 (au lieu de 400) pour garder les doubles et plus de FPS
	double			osY = 0;
	double			c = 0;

	GLuint u_resolution  = glGetUniformLocation( _program.getId(), "u_resolution");
	GLuint zoom  = glGetUniformLocation( _program.getId(), "zoom");
	GLuint offsetX  = glGetUniformLocation( _program.getId(), "offsetX");
	GLuint offsetY  = glGetUniformLocation( _program.getId(), "offsetY");
	GLuint limit  = glGetUniformLocation( _program.getId(), "limit");
	GLuint cr  = glGetUniformLocation( _program.getId(), "cr");
	glUniform2d(u_resolution, WIDTH, HEIGHT);

	auto last = std::chrono::high_resolution_clock::now();
	int frameCount = 0;
	double fpsTimer = 0.0;
	double currentFPS = 0.0;
	
	while ( running )
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
					z *= 1.15;
				else if (e.wheel.y > 0)
					z *= 0.85;
				osX = mouseWorldXBefore - (mouseX * z);
  				osY = mouseWorldYBefore - (mouseY * z);
			}
			if ( e.type == SDL_KEYDOWN )
			{
				if ( e.key.keysym.sym == 0x006d) // 'm'
					f += 10;
				if ( e.key.keysym.sym == 0x006e) // 'n'
				{
					int mouseX, mouseY;
					SDL_GetMouseState( &mouseX, &mouseY );
					
					mouseY -= HEIGHT;
					mouseY *= -1;

					// Store current state
					_zoomStart = z;
					_offsetXStart = osX;
					_offsetYStart = osY;
					
					double mouseWorldX = osX + (mouseX * z);
					double mouseWorldY = osY + (mouseY * z);
					
					// Target: zoom in by factor of 5 centered on mouse
					_zoomTarget = z * 0.5;
					_offsetXTarget = mouseWorldX - (mouseX * _zoomTarget);
					_offsetYTarget = mouseWorldY - (mouseY * _zoomTarget);
					
					// Start animation
					_isZooming = true;
					_zoomElapsedTime = 0.0;
				}
			}
		}
		
		// Update smooth zoom animation
		if (_isZooming)
		{
			_zoomElapsedTime += dt;
			
			if (_zoomElapsedTime >= _zoomDuration)
			{
				// Animation complete
				z = _zoomTarget;
				osX = _offsetXTarget;
				osY = _offsetYTarget;
				_isZooming = false;
				_zoomElapsedTime = 0.0;
			}
			else
			{
				// Linear interpolation (t between 0 and 1)
				double t = _zoomElapsedTime / _zoomDuration;
				
				// Apply easing function (ease-out cubic for smoother effect)
				double easeT = 1.0 - (1.0 - t) * (1.0 - t) * (1.0 - t);
				
				z = _zoomStart + (_zoomTarget - _zoomStart) * easeT;
				osX = _offsetXStart + (_offsetXTarget - _offsetXStart) * easeT;
				osY = _offsetYStart + (_offsetYTarget - _offsetYStart) * easeT;
			}
		}
		
		c += 0.0001;
		
		// Limitation intelligente des itérations selon le zoom
		// Formule: base + log(zoom) pour une augmentation progressive
		// Reste plafonnée à 200 pour garder les FPS avec doubles
		int dynamicLimit = (int)(100 + 30.0 * log(1.0 / z + 1.0));
		if (dynamicLimit > 200) dynamicLimit = 200;
		if (dynamicLimit < f) dynamicLimit = f;  // Toujours >= itérations modifiées par utilisateur
		
		glUniform1i( limit, dynamicLimit );
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


	Camera	camera(1.45,-30,0);


	GLuint cam_pos	= glGetUniformLocation( _program.getId(), "cam_pos" );
	GLuint cam_dir	= glGetUniformLocation( _program.getId(), "forward" );
	GLuint right	= glGetUniformLocation( _program.getId(), "right" );
	GLuint up		= glGetUniformLocation( _program.getId(), "up" );
	GLuint ViewportResolution	= glGetUniformLocation( _program.getId(), "resolution" );
	GLuint ScreenResolution	= glGetUniformLocation( _program.getId(), "ScreenResolution" );

	while (running)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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

		
		glUniform3d( cam_pos, camera.position._x , camera.position._y , camera.position._z );
		glUniform3d( cam_dir, camera.front._x , camera.front._y , camera.front._z );
		glUniform3d( up , camera.up._x, camera.up._y , camera.up._z);
		glUniform3d( right , camera.right._x, camera.right._y , camera.right._z );
		glUniform2d( ViewportResolution, camera.w, camera.h);
		glUniform2d( ScreenResolution, WIDTH, HEIGHT);

		_fractal.drawShape( _win );
		//SDL_Delay(20);
		

	}
	return ( true );
}

App::~App()
{
	
}