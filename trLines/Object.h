#ifndef __OBJECT_H__
#define __OBJECT_H__

#include "LineSeg.h"
#include "ByteImg.h"
#include "FloatImg.h"

namespace app {

struct IntersectData {
	real t;
	Point p;
	Point normal;
};

class Object {
public:
	virtual bool IsIntersect( const LineSeg & ray, IntersectData & data ) = 0;
	virtual void Draw( ByteImg & data ) = 0;

	void SetColor( Color3 color );
	Color3 GetColor() const;
	app::ColorFloat GetFloatColor() const;
protected:
	Color3 objectColor;

};

}

#endif