#ifndef __FLOATIMG_H__
#define __FLOATIMG_H__

namespace app {

struct ColorFloat {
	ColorFloat() : b( 0.0 ), g( 0.0 ), r( 0.0 ) {}
	ColorFloat( real b_, real g_, real r_ ) : b( b_ ), g( g_ ), r( r_ ) {}

	real b;
	real g;
	real r;

	ColorFloat & operator+=( const ColorFloat & rhs );
};

ColorFloat operator+( const ColorFloat & lhs, const ColorFloat & rhs );
ColorFloat operator*( const ColorFloat & lhs, const ColorFloat & rhs );
ColorFloat operator*( const ColorFloat & lhs, real t );
ColorFloat operator/( const ColorFloat & lhs, real t );

class FloatImg {
public:
	FloatImg( UINT width, UINT height );
	FloatImg( UINT width, UINT height, std::vector<BYTE> img, std::vector<BYTE> ocl_ );
	FloatImg( const std::wstring & filename, const std::wstring & ocl_fn );

	void SetPixel( UINT x, UINT y, const ColorFloat & c );
	ColorFloat GetPixel( UINT x, UINT y ) const;

	void SetBufferPixel( UINT x, UINT y, const ColorFloat & c );
	ColorFloat GetBufferPixel( UINT x, UINT y ) const;

	bool IsOccluded( UINT x, UINT y ) const;

	UINT GetWidth() const;
	UINT GetHeight() const;

	void SetExposure( real exposure_ );
	void SetWhite( real white_ );
	void SetFilterRadius( UINT fradius );
	
	void FilterBuffer( UINT radius );

	std::vector<BYTE> ConvertToBytes();
	real Luminance( UINT x, UINT y ) const;
	real GetLogAverageLuminance() const;
private:
	UINT GetPixelIndex( UINT x, UINT y ) const;
private:
	std::vector<real> raw_data; // Исходный уровень
	std::vector<real> buffer; // Буффер
	mutable std::vector<bool> ocl; // 
	std::vector<BYTE> testtest;

	real exposure;
	real white;
	UINT filter_radius;

	UINT stride;
	UINT w, h;
};

}

#endif