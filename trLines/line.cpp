#include "stdafx.h"
#include "shape.h"
#include "line.h"

namespace app {

Line::Line()
{ }

Line::Line( const Gdiplus::PointF & point1, const Gdiplus::PointF & point2 )
	: p1( point1 ), p2( point2 )
{ }

Line::Line( real x1, real y1, real x2, real y2 )
	: p1( x1, y1 ), p2( x2, y2 )
{ }

void Line::Draw( Gdiplus::Graphics & gr )
{
	Gdiplus::Pen p( Gdiplus::Color( 0, 0, 0 ) );
	gr.DrawLine( &p, p1, p2 );
}

}