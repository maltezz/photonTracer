#ifndef __TESTLINE_H__
#define __TESTLINE_H__

#include "Object.h"
#include "LineStrategy.h"

namespace app {

class TestLine : public Object {
public:
//	TestLine();
//	TestLine( const Gdiplus::PointF & point1, const Gdiplus::PointF & point2 );
	TestLine( real x1, real y1, real x2, real y2, std::shared_ptr<LineStrategy> alg );

	bool IsIntersect( const LineSeg & ray, IntersectData & data );
	void Draw( ByteImg & data );
private:
	app::Point p1, p2;
	LineSeg segment;
	Point normal; // unit normal
	std::shared_ptr<LineStrategy> line_alg;
};


}
#endif