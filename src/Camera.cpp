#include "Camera.hpp"

Camera::Camera() : position(0, 0, 0), front(1, 0, 0), fov(70)
{
	h = 2 * tan( fov / 2 * (M_PI * 180 ));
	w = h * ( (double) WIDTH / (double) HEIGHT );
	right = get_right_local_vector();
	up = get_up_local_vector();
}

Vec3	Camera::get_up_local_vector( void )
{
	Vec3	up;

	up = Vec3::cross( front, right );
	up.normalize();
	return ( up );
}


Vec3	Camera::get_right_local_vector( void )
{
	Vec3	right;
	Vec3	up_world( 0, 0, 1 );

	if ( Vec3::dot( front, up_world ) == 1)
		up_world = Vec3( 0, 1, 0 );
	right = Vec3::cross( front, up_world );
	right.normalize();
	return ( right );  
}


Camera::Camera( double x, double y, double z ) : position( x, y, z ), front( 1, 0, 0 ), fov( 70 )
{

}

Camera::~Camera()
{

}