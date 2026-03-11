#include "Camera.hpp"

Camera::Camera() : position(0, 0, 0), front(1, 0, 0), fov(70)
{
	midIsPress = false;
	shiftIsPress = false; 
	h = 2 * tan( fov / 2 * (M_PI * 180 ));
	w = h * ( (double) WIDTH / (double) HEIGHT );
	right = get_right_local_vector();
	up = get_up_local_vector();
}

Vec3	Camera::get_up_local_vector( void )
{
	Vec3	up;

	up = Vec3::cross( right, front );

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

Camera::Camera( double x, double y, double z ) : position( x, y, z ), front( 0, 1, 0 ), fov( 70 )
{
	midIsPress = false;
	shiftIsPress = false; 
	h = 2 * tan( fov / 2 * (M_PI / 180 ));
	w = h * ( (double) WIDTH / (double) HEIGHT );
	right = get_right_local_vector();
	up = get_up_local_vector();
	std::cout << " right : " << right << std::endl;
	std::cout << " up : " << up << std::endl;
	std::cout << " front : " << front << std::endl;
	std::cout << " cam_pos : " << position << std::endl;
	std::cout << "[ ViewportResolution ] w : " << w << " h : " << h << std::endl; 
}

void	Camera::moveForward( void )
{
	Vec3 frontmp = front;
	position = position + frontmp / 20;
}

void	Camera::moveBackward( void )
{
	position = position - front;
}

bool	is_a_bad_rotation(Vec3 v_new, Vec3 v_old)
{
	if ((v_old._x > 0 && v_new._x < 0)
		&& (v_old._y < 0 && v_new._y > 0) && (v_old._z > 0.99 || v_old._z < 0.01))
		return (true);
	if ((v_old._x < 0 && v_new._x > 0)
		&& (v_old._y > 0 && v_new._y < 0) && (v_old._z > 0.99 || v_old._z < 0.01))
		return (true);
	if ((v_old._x > 0 && v_new._x > 0)
		&& (v_old._x < 0 && v_new._y < 0) && (v_old._z > 0.99 || v_old._z < 0.01))
		return (true);
	if ((v_old._x > 0 && v_old._y > 0)
		&& (v_new._x < 0 && v_new._y < 0) && (v_old._z > 0.99 || v_old._z < 0.01))
		return (true);
	if ((v_old._x < 0 && v_old._y < 0)
		&& (v_new._x > 0 && v_new._y > 0) && (v_old._z > 0.99 || v_old._z < 0.01))
		return (true);
	return (false);
}

void	Camera::handleMouseMidButtonPress( double x, double y )
{
		midIsPress = true;
		prev_x = x;
		prev_y = y;
}

void	Camera::handleMouseMovement( double x, double y )
{
	double dx = prev_x - x;
	double dy = prev_y - y;
	Vec3 r = right;
	Vec3 u = up;
	double	yaw;
	double	pitch;
	Vec3	frontTmp;

	prev_x = x;
	prev_y = y;
	if ( true == midIsPress)
	{
		if (true == shiftIsPress)
		{
			yaw = dx * -0.002;
			pitch = dy * -0.002;
			frontTmp = front;
			front = Vec3::vecRotate(front, Vec3(0, 0, 1), -yaw);
			right = get_right_local_vector();
			up = get_up_local_vector();
			front = Vec3::vecRotate(front, right, -pitch);
			front.normalize();
			right = get_right_local_vector();
			up = get_up_local_vector();
			if (is_a_bad_rotation(front, frontTmp))
				front = frontTmp;
		}
		else
		{

			position = position + r * ( dx * 0.1 ) + u * ( -dy * 0.1 );
		}
	}
}

// void	Camera::midMouseButtonPress();
// void	Camera::midMouseButtonRelease();

Camera::~Camera()
{

}