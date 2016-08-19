#include "stdafx.h"
#include "Rectangle.h"

namespace app {

Rectangle::Rectangle()
	: rect( 0.0f, 0.0f, 10.0f, 10.0f )
{}

Rectangle::Rectangle( const Gdiplus::RectF & r )
	: rect( r )
{}

void Rectangle::SetRect( Gdiplus::RectF r )
{
	rect = r;
}

void Rectangle::Draw( Gdiplus::Graphics & gr )
{
	Gdiplus::Pen p( Gdiplus::Color( 0, 0, 0 ) );
	gr.DrawRectangle( &p, rect );
}

}