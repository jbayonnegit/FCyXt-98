#include <Vec3.hpp>

Vec3::Vec3()
{

}

Vec3::Vec3(double x, double y, double z) : _x( x ), _y( y ), _z( z )
{

}

Vec3::Vec3(const Vec3 &other)
{
	_x = other._x;
	_y = other._y;
	_z = other._z;
}

Vec3 &Vec3::operator=(const Vec3 &other)
{
	if (this != &other)
	{
		_x = other._x;
		_y = other._y;
		_z = other._z;
	}
	return (*this);
}

Vec3 &Vec3::operator+(const Vec3 &other)
{
	if (this != &other)
	{
		_x += other._x;
		_y += other._y;
		_z += other._z;
	}
	return (*this);
}

Vec3 &Vec3::operator-(const Vec3 &other)
{
	if (this != &other)
	{
		_x -= other._x;
		_y -= other._y;
		_z -= other._z;
	}
	return (*this);
}

Vec3::~Vec3()
{

}


void	Vec3::normalize( void )
{
	double	norme;

	norme = sqrt( pow(_x, 2) + pow(_y, 2) + pow(_z, 2) );
	_x /= norme;
	_y /= norme;
	_z /= norme;
}