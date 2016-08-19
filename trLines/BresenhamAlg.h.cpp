#include "stdafx.h"
#include "BresenhamAlg.h"

namespace app {

bool BresenhamAlg::DrawLine( FloatImg & data, LineSeg & line, ColorFloat & color )
{
	int x0 = ( int ) line.p1.x;
	int x1 = x0 + ( int ) line.p2.x;
	int y0 = ( int ) line.p1.y;
	int y1 = y0 + ( int ) line.p2.y;
	bool steep = false;
	if ( std::abs( x1 - x0 ) < std::abs( y1 - y0 ) ) {
		std::swap( y0, x0 );
		std::swap( y1, x1 );
		steep = true;
	}

	int error = 0;
	int y = y0;
	int dy = y1 - y0;
	int dx = x1 - x0;

	int sign, slope;
	bool forward = x0 < x1;
	if ( forward ) {
		sign = ( dx < 0 ) ? -1 : 1;
		slope = ( dy * dx < 0 ) ? -1 : 1;
	}
	else {
		sign = ( dx > 0 ) ? -1 : 1;
		slope = ( dy * dx > 0 ) ? -1 : 1;
	}
	dx = std::abs( dx );

	int last_x = 0, last_y = 0;
	int x = x0;
	while ( true ) {
		if ( steep ) {
			if ( data.IsOccluded( y, x ) ) {
				color = color * data.GetPixel( y, x );
				line.p1 = { ( real )last_x, ( real )last_y };
				return true;
			}
			last_x = y;
			last_y = x;
		} else {
			if ( data.IsOccluded( x, y ) ) {
				color = color * data.GetPixel( x, y );
				line.p1 = { ( real )last_x, ( real )last_y };
				return true;
			}
			last_x = x;
			last_y = y;
		}

		data.SetBufferPixel( last_x, last_y, Accumulate( data, last_x, last_y, color ) );

		error += dy * sign;
		if ( std::abs( error << 1 ) >= dx ) {
			y += slope;
			error += -slope * dx;
		}

		if ( forward ) {
			++x;
			if ( x > x1 ) break;
		} else {
			--x;
			if ( x < x1 ) break;
		}
	}

	return false;
}

ColorFloat BresenhamAlg::Accumulate( const FloatImg & data, int x, int y, app::ColorFloat color ) const
{
	return ( data.GetBufferPixel( x, y ) + color );
}

}