#ifndef __POINT_H__
#define __POINT_H__

namespace app {

struct Point {
	Point( ) : x( 0 ), y( 0 ) {}
	Point( real x_, real y_ ) : x( x_ ), y( y_ ) {}

	// |p1|*|p2|*sin(theta)
	static real cross( const Point & p1, const Point & p2 );
	// |p1|*|p2|*cos(theta)
	static real dot( const Point & p1, const Point & p2 );

	void Normalize();

	Point& operator-=( const Point& rhs );
	Point& operator+=( const Point& rhs );
	Point& operator/=( real t );
	Point& operator*=( real t );

	real x, y;
};

Point operator-( const Point& lhs, const Point& rhs );
Point operator+( const Point& lhs, const Point& rhs );
Point operator*( const Point& lhs, real rhs );
Point operator-( const Point& lhs );

}

#endif