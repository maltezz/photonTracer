#include "stdafx.h"
#include "Border.h"
#include "Rectangle.h"

namespace app {

Border::Border( std::unique_ptr<Shape> sh )
{
	shape = std::move( sh );
}

void Border::SetClipRegion( Gdiplus::Rect clip_r_ )
{
	clip_r = clip_r_;
	static_cast<app::Rectangle*>( shape.get() )->SetRect( Gdiplus::RectF( ( real ) clip_r.X, ( real ) clip_r.Y, ( real ) clip_r.Width - 1, ( real ) clip_r.Height - 1 ) );
}

Gdiplus::Rect Border::GetClipRegion() const
{
	return clip_r;
}

void Border::Draw( Gdiplus::Graphics & gr )
{
	gr.SetClip( clip_r );
	shape->Draw( gr );
}

}