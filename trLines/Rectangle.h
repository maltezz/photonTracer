#ifndef __RECTANGLE_H__
#define __RECTANGLE_H__

#include "shape.h"

namespace app {

class Rectangle : public Shape {
public:
	Rectangle();
	Rectangle( const Gdiplus::RectF & r );

	// FIXME: недостаток дизайна?
	void SetRect( Gdiplus::RectF r );

	void Draw( Gdiplus::Graphics & gr );
private:
	Gdiplus::RectF rect;
};

}
#endif