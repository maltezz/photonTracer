#ifndef __SHAPE_H__
#define __SHAPE_H__

namespace app {

class Shape {
public:
	virtual void Draw( Gdiplus::Graphics & gr ) = 0;
};

}

#endif