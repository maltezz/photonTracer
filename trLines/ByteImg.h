#ifndef __BYTEIMG_H__
#define __BYTEIMG_H__

#include "FloatImg.h"

namespace app {

struct Color3 {
	BYTE b;
	BYTE g;
	BYTE r;
};

class ByteImg {
public:
	ByteImg( int width, int height );
	ByteImg( int width, int height, std::vector<BYTE> data );

	void SaveImage( const WCHAR * filename );

	void SetPixel( int x, int y, const Color3 & c );
	void SetPixel( int x, int y, const ColorFloat & c );
	Color3 GetPixel( int x, int y ) const;

	static int GetEncoderClsid( const WCHAR* format, CLSID* pClsid );
private:
	int GetPixelIndex( int x, int y ) const;
private:
	Gdiplus::Bitmap image;
	Gdiplus::BitmapData image_data;
	BYTE * raw_data;
};

}

#endif