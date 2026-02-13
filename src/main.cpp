# include "App.hpp"

int	main(int argc, char **argv)
{
	(void)argc;
	App	application( argv[1] );
	
	application.run();
}
