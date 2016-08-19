#ifndef __BORDER_H__
#define __BORDER_H__

#include "shape.h"

namespace app {

class Border {
public:
	Border( std::unique_ptr<Shape> sh );

	void SetClipRegion( Gdiplus::Rect clip_r_ );
	Gdiplus::Rect GetClipRegion() const;

	void Draw( Gdiplus::Graphics & gr );
private:
	std::unique_ptr<Shape> shape;
	Gdiplus::Rect clip_r;
};

}
#endif