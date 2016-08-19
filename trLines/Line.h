#ifndef __LINE_H__
#define __LINE_H__

#include "shape.h"

namespace app {

class Line : public Shape {
public:
	Line();
	Line( const Gdiplus::PointF & point1, const Gdiplus::PointF & point2 );
	Line( real x1, real y1, real x2, real y2 );

	void Draw( Gdiplus::Graphics & gr );
private:
	std::shared_ptr<Gdiplus::Pen> pen;
	Gdiplus::PointF p1, p2;
};

}
#endif