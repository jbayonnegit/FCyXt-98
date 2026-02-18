# include "App.hpp"

int	main(int argc, char **argv)
{
	(void)argc;
	if ( argc != 2 )	
		return ( 1 );

	App	application( argv[1] );
	
	application.run();
}
