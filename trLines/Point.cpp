#include "stdafx.h"
#include "Point.h"

namespace app {

real Point::cross( const Point & p1, const Point & p2 )
{
	return p1.x * p2.y - p1.y * p2.x;
}

real Point::dot( const Point & p1, const Point & p2 )
{
	return p1.x * p2.x + p1.y * p2.y;
}

void Point::Normalize()
{
	*this /= sqrt( dot( *this, *this ) );
}

Point & app::Point::operator-=( const Point & rhs )
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Point & Point::operator+=( const Point & rhs )
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Point & Point::operator/=( real t )
{
	x /= t;
	y /= t;
	return *this;
}

Point & Point::operator*=( real t )
{
	x *= t;
	y *= t;
	return *this;
}

Point operator-( const Point & lhs, const Point & rhs )
{
	Point out = lhs;
	out -= rhs;
	return out;
}

Point operator+( const Point & lhs, const Point & rhs )
{
	Point out = lhs;
	out += rhs;
	return out;
}

Point operator*( const Point & lhs, real rhs )
{
	Point out = lhs;
	out *= rhs;
	return out;
}

Point operator-( const Point & lhs )
{
	Point out = lhs;
	out.x = -out.x;
	out.y = -out.y;
	return out;
}

}