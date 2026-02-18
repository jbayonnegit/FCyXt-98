#include "Vec3.hpp"
#include <cmath> // tan

# define	HEIGHT 1080
# define	WIDTH 1920

class Camera
{
	private :

		Vec3	get_right_local_vector( void );
		Vec3	get_up_local_vector( void );
	
	public:

		Vec3 	position;
		Vec3 	front;
		Vec3 	right;
		Vec3 	up;
		double	h;
		double	w;
		double	fov;
		Camera();
		Camera(double x, double y, double z);
		~Camera();
};
