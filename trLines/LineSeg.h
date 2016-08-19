#ifndef __LINESEG_H__
#define __LINESEG_H__

#include "Point.h"

namespace app {

struct LineSeg {
	LineSeg() {};
	LineSeg( app::Point p1_, app::Point p2_ );

	static bool isIntersect( const LineSeg & l1, const LineSeg & l2, Point & out_p, real & t_ );

	Point p1;
	Point p2; // end point = p2 - p1
};

}

#endif