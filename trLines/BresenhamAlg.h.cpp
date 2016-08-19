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

	if ( x0 > x1 )
		return DrawLineBackward( data, x0, y0, x1, y1, steep, line, color );
	else
		return DrawLineForward( data, x0, y0, x1, y1, steep, line, color );
}

bool BresenhamAlg::DrawLineForward( FloatImg & data, int x0, int y0, int x1, int y1, bool steep, LineSeg & line, ColorFloat & color )
{
	int error = 0;
	int y = y0;
	int dy = y1 - y0;
	int dx = x1 - x0;
	int sign = ( dx < 0 ) ? -1 : 1;
	int slope = ( dy * dx < 0 ) ? -1 : 1;
	dx = std::abs( dx );

	int last_x = 0, last_y = 0;

	for ( int x = x0; x <= x1; ++x ) {
		ColorFloat curr_c = color;// / ( dist );
		if ( steep ) {
			if ( data.IsOccluded( y, x ) ) {
				color = color * data.GetPixel( y, x );
				line.p1 = { (real)last_x, (real)last_y };
				return true;
			}
			last_x = y;
			last_y = x;
			data.SetBufferPixel( y, x, Accumulate( data, y, x, curr_c ) ); //-------------------------------
		}
		else {
			if ( data.IsOccluded( x, y ) ) {
				color = color * data.GetPixel( x, y );
				line.p1 = { ( real )last_x, ( real )last_y };
				return true;
			}
			last_x = x;
			last_y = y;
			data.SetBufferPixel( x, y, Accumulate( data, x, y, curr_c ) ); //-------------------------------
		}

		error += dy * sign;
		if ( std::abs( error << 1 ) >= dx ) {
			y += slope;
			error += -slope * dx;
		}
	}
	return false;
}

bool BresenhamAlg::DrawLineBackward( FloatImg & data, int x0, int y0, int x1, int y1, bool steep, LineSeg & line, ColorFloat & color )
{
	int error = 0;
	int y = y0;
	int dy = y1 - y0;
	int dx = x1 - x0;
	int sign = ( dx > 0 ) ? -1 : 1;
	int slope = ( dy * dx > 0 ) ? -1 : 1;
	dx = std::abs( dx );

	int last_x = x0, last_y = y;

	for ( int x = x0; x >= x1; --x ) {
		if ( steep ) {
			if ( data.IsOccluded( y, x ) ) {
				color = color * data.GetPixel( y, x );
				line.p1 = { ( real ) last_x, ( real ) last_y };
				return true;
			}
			last_x = y;
			last_y = x;
			data.SetBufferPixel( y, x, Accumulate( data, y, x, color ) );
		}
		else {
			if ( data.IsOccluded( x, y ) ) {
				color = color * data.GetPixel( x, y );
				line.p1 = { ( real ) last_x, ( real ) last_y };
				return true;
			}
			last_x = x;
			last_y = y;
			data.SetBufferPixel( x, y, Accumulate( data, x, y, color ) );
		}

		error += dy * sign;
		if ( std::abs( error << 1 ) >= dx ) {
			y += slope;
			error += -slope * dx;
		}
	}
	return false;
}

ColorFloat BresenhamAlg::Accumulate( const FloatImg & data, int x, int y, app::ColorFloat color ) const
{
	return ( data.GetBufferPixel( x, y ) + color );
}

}