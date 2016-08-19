#ifndef __LINESTRATEGY_H__
#define __LINESTRATEGY_H__

#include "Point.h"
#include "ByteImg.h"
#include "FloatImg.h"
#include "LineSeg.h"

namespace app {

class LineStrategy {
public:
	virtual bool DrawLine( FloatImg & data, LineSeg & line, app::ColorFloat & color ) = 0;
};

}

#endif