#ifndef __BRESENHAMALG_H__
#define __BRESENHAMALG_H__

#include "LineStrategy.h"

namespace app {

class BresenhamAlg : public LineStrategy {
public:
	bool DrawLine( FloatImg & data, LineSeg & line, ColorFloat & color );
private:
	ColorFloat BresenhamAlg::Accumulate( const FloatImg & data, int x, int y, app::ColorFloat color ) const;
};

}

#endif