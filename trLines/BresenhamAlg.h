#ifndef __BRESENHAMALG_H__
#define __BRESENHAMALG_H__

#include "LineStrategy.h"

namespace app {

class BresenhamAlg : public LineStrategy {
public:
	// TODO: необходим рефакторинг
	bool DrawLine( FloatImg & data, LineSeg & line, ColorFloat & color );
private:
	ColorFloat BresenhamAlg::Accumulate( const FloatImg & data, int x, int y, app::ColorFloat color ) const;
	bool DrawLineForward( FloatImg & data, int x0, int y0, int x1, int y1, bool steep, LineSeg & line, ColorFloat & color );
	bool DrawLineBackward( FloatImg & data, int x0, int y0, int x1, int y1, bool steep, LineSeg & line, ColorFloat & color );
};

}

#endif