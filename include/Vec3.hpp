#include <cmath> //pow sqrt
#include <iostream> //cout

class Vec3
{
	public:
		double	_x, _y, _z;
		Vec3();
		Vec3(double x, double y, double z);
		Vec3(const Vec3 &other);
		Vec3 &operator=(const Vec3 &other);
		Vec3 &operator+(const Vec3 &other);
		Vec3 &operator-(const Vec3 &other);
		Vec3 &operator*(const Vec3 &other);
		Vec3 &operator/(const Vec3 &other);

		~Vec3();

		static double	dot( const Vec3 &a, const Vec3 &b )
		{
			return (  a._x * b._x +  a._y * b._y  + a._z * b._z );
		}

		static Vec3	cross( const Vec3 &a, const Vec3 &b )
		{
			return (Vec3(a._y * b._z - a._z * b._y,
						 a._z * b._x - a._x * b._z,
						 a._x * b._y - a._y * b._x));
		}

		static Vec3	scale( const Vec3 &a, const double scalar )
		{
			return ( Vec3( a._x * scalar, a._y * scalar, a._z * scalar) );
		}

		void	normalize( void );
};
std::ostream	&operator<<( std::ostream &stream, Vec3 &vec);
 