#ifndef __APP_H__
#define __APP_H__

#include "LineStrategy.h"

namespace app {

struct InitStruct {
	std::wstring level_file;
	std::wstring ocl_level_file;
	std::wstring out_level_file;
	UINT light_x, light_y;
	ColorFloat light_color;
	UINT num_rays;
	UINT filter;
	real exposure;
	real white;
};

class App {
public:
	App() {}

	void Initialize( InitStruct params_, std::shared_ptr<LineStrategy> alg );
	void RayTracing();
	void SaveImage();
private:
	Point Reflect( const Point & ray, const Point & normal );
	void GenerateTraceLine( LineSeg & ray ) const;
	LineSeg GenerateTraceLine( const Point & p1, const Point & p2 ) const;
	void TraceLine( LineSeg ray, ColorFloat color );
	int GetEncoderClsid( const WCHAR* format, CLSID* pClsid );
private:
	InitStruct params;
	std::shared_ptr<LineStrategy> line_alg;
	real hypotenuse;

	LineSeg borders[4]; // 0 - left, 1 - right, 2 - top, 3 - bottom

	std::mt19937 rand_engine;
	std::uniform_real<real> u_rand;

	std::unique_ptr<FloatImg> float_image;
};

}
#endif