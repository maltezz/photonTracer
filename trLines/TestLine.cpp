#include "stdafx.h"
#include "TestLine.h"

namespace app {

TestLine::TestLine( real x1, real y1, real x2, real y2, std::shared_ptr<LineStrategy> alg )
	:p1( Point( x1, y1 ) ), p2( Point( x2, y2 ) ), segment( p1, p2 )
{ 
	normal = Point( -segment.p2.y, segment.p2.x );
	normal.Normalize();
	line_alg = alg;
}

bool TestLine::IsIntersect( const LineSeg & ray, IntersectData & data )
{
	Point pt;
	float t;
	// TODO: pt может заменить на data.p?
	bool result = LineSeg::isIntersect( segment, ray, pt, t );
	if ( result ) {
		data.p = pt;
		data.t = t;
		// TODO - вынести? оптимизировать?
		float dotp = Point::dot( ray.p2, normal );
		if ( dotp > 0 )
			data.normal = -normal;
		else data.normal = normal;
		return true;
	}
	return false;
}

void TestLine::Draw( ByteImg & data )
{
//	line_alg->DrawLine( data, p1.x, p1.y, p2.x, p2.y, objectColor );
}

}