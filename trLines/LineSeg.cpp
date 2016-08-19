#include "stdafx.h"
#include "LineSeg.h"

namespace app {

LineSeg::LineSeg( Point p1_, Point p2_ )
	: p1(p1_)
{
	p2 = p2_ - p1_;
}

bool LineSeg::isIntersect( const LineSeg & l1, const LineSeg & l2, Point & out_p, real & t_ )
{
	// p - l1.origin, r - l1.dir
	// q - l2.origin, s - l2.dir
	const real eps = (real)0.01;
	real r_dot_s = Point::cross( l1.p2, l2.p2 );
	Point qp = l2.p1 - l1.p1;
	// коллинеарные отрезки ( на одной прямой считаю не пересекающимися )
	if ( abs( r_dot_s ) < eps ) { // r*s = 0 && (q-p)*r = or != 0
		return false;
	}
	else {
		real t = Point::cross(qp, l2.p2) / r_dot_s;
		real u = Point::cross(qp, l1.p2 ) / r_dot_s;
		if ( t >= 0 && t <= 1 && u >= 0 && u <= 1 ) {
			out_p = l1.p1 + l1.p2 * t;
			t_ = u;
			return true;
		}
	}
	return false;
}

}