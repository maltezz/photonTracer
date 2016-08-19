#include "stdafx.h"
#include "FloatImg.h"

namespace app {

FloatImg::FloatImg( UINT width, UINT height )
	:w(width), h(height)
{
	size_t sz = sizeof real;
	stride = 3 * width;
	raw_data.resize( 3 * width * height );
	ocl.resize( width * height );
	for ( UINT i = 0; i < raw_data.size(); ++i )
		raw_data[i] = 0.0f;
}

FloatImg::FloatImg( const std::wstring & filename, const std::wstring & ocl_fn )
{
	Gdiplus::Bitmap img( filename.c_str() );
	w = img.GetWidth();
	h = img.GetHeight();
	stride = 3 * w;
	Gdiplus::BitmapData data;
	img.LockBits( &Gdiplus::Rect( 0, 0, w, h ), Gdiplus::ImageLockModeWrite, PixelFormat24bppRGB, &data );
	raw_data.resize( 3 * w * h );
	BYTE * d = ( BYTE * )data.Scan0;
	for ( UINT i = 0; i < raw_data.size(); ++i ) {
		BYTE dd = d[i];
		raw_data[i] = ( real ) dd / 255.0;
	}

	Gdiplus::Bitmap ocl_img( ocl_fn.c_str() );
	Gdiplus::BitmapData ocl_data;
	ocl_img.LockBits( &Gdiplus::Rect( 0, 0, w, h ), Gdiplus::ImageLockModeWrite, PixelFormat24bppRGB, &ocl_data );
	ocl.resize( w * h );
	d = ( BYTE * )ocl_data.Scan0;
	for ( UINT i = 0; i < raw_data.size(); i += 3 )
		if ( d[i] == 255 )
			ocl[i / 3] = true;

	buffer.resize( 3 * w * h );
	// add ambient light
	std::fill( buffer.begin(), buffer.end(), 0.3f );
}


FloatImg::FloatImg( UINT width, UINT height, std::vector<BYTE> img, std::vector<BYTE> ocl_ )
{
	raw_data.resize( img.size() );
	for ( UINT i = 0; i < img.size(); ++i )
		raw_data[i] = ( real )img[i] / 255.0;

	ocl.reserve( ocl_.size() / 3 );
	for ( UINT i = 0; i < ocl_.size(); i += 3 )
		if ( ocl_[i] > 0 )
			ocl.push_back( false );
		else ocl.push_back( true );
}


void FloatImg::SetPixel( UINT x, UINT y, const ColorFloat & c )
{
	UINT ind = GetPixelIndex( x, y );
	raw_data[ind] = c.r;
	raw_data[ind+1] = c.g;
	raw_data[ind+2] = c.b;
}

ColorFloat FloatImg::GetPixel( UINT x, UINT y ) const
{
	UINT ind = GetPixelIndex( x, y );
	return{ raw_data[ind], raw_data[ind + 1],raw_data[ind + 2] };
}

void FloatImg::SetBufferPixel( UINT x, UINT y, const ColorFloat & c )
{
	UINT ind = GetPixelIndex( x, y );
	buffer[ind] = c.r;
	buffer[ind + 1] = c.g;
	buffer[ind + 2] = c.b;
}

ColorFloat FloatImg::GetBufferPixel( UINT x, UINT y ) const
{
	UINT ind = GetPixelIndex( x, y );
	return{ buffer[ind], buffer[ind + 1],buffer[ind + 2] };
}

bool FloatImg::IsOccluded( UINT x, UINT y ) const
{
	UINT ind = y * w + x;
	return ocl[ind];
}

UINT FloatImg::GetWidth() const
{
	return w;
}

UINT FloatImg::GetHeight() const
{
	return h;
}

void FloatImg::SetExposure( real exposure_ )
{
	exposure = exposure_;
}

void FloatImg::SetWhite( real white_ )
{
	white = white_;
}

void FloatImg::SetFilterRadius( UINT fradius )
{
	filter_radius = fradius;
}

std::vector<BYTE> FloatImg::ConvertToBytes()
{
	std::vector<BYTE> out;
	out.resize( 3 * w * h );

	if ( filter_radius > 0 )
		FilterBuffer( filter_radius );

	for ( UINT i = 0; i < raw_data.size(); i += 3 ) {
		real a = ocl[i/3] ? 1.0 : 0.0;
		raw_data[i] *= ( buffer[i] + a );
		raw_data[i+1] *= ( buffer[i+1] + a );
		raw_data[i+2] *= ( buffer[i+2] + a );
	}

	real avg_lum = GetLogAverageLuminance();

	for ( UINT x = 0; x < w; ++x ) {
		for ( UINT y = 0; y < h; ++y ) {
			real lp = Luminance( x, y );
			real l = exposure / avg_lum * lp;
			real ld = l * ( 1.0 + l / ( white * white ) ) / ( 1.0 + l );
			real scale = ld / lp;
			UINT ind = 3 * w * y + x * 3;
			ColorFloat pixel = GetPixel( x, y );
			real c1 = 255.0 * min( 1.0, pixel.b * scale );
			real c2 = 255.0 * min( 1.0, pixel.g * scale );
			real c3 = 255.0 * min( 1.0, pixel.r * scale );
			out[ind] = ( BYTE )( c1 ); // b
			out[ind + 1] = ( BYTE )( c2 );
			out[ind + 2] = ( BYTE )( c3 );
		}
	}

	return out;
}

real FloatImg::Luminance( UINT x, UINT y ) const
{
	ColorFloat c = GetPixel( x, y );
	return 0.2126 * c.r + 0.7152 * c.g + 0.0722 * c.b;
}

real FloatImg::GetLogAverageLuminance() const
{
	real delta = 0.001;
	real ln_acc = 0.0;
	for ( UINT x = 0; x < w; ++x ) {
		for ( UINT y = 0; y < h; ++y ) {
			ln_acc += std::log( delta + Luminance( x, y ) );
		}
	}

	ln_acc /= raw_data.size() / 3;

	return std::exp( ln_acc );;
}

UINT FloatImg::GetPixelIndex( UINT x, UINT y ) const
{
	return y * stride + x * IMAGE_BPP;
}

// Простой box filter
void FloatImg::FilterBuffer( UINT radius )
{
	UINT r = 3 + ( radius - 1 ) * 2;
	for ( int x = 0; x < w; ++x )
		for ( int y = 0; y < h; ++y ) {
			ColorFloat sum;
			for ( int xi = x - ( int )radius; xi <= x + ( int )radius; ++xi )
				for ( int yi = y - ( int )radius; yi <= y + ( int )radius; ++yi )
					if ( xi >= 0 && xi < w && yi >= 0 && yi < h )
						sum += GetBufferPixel( xi, yi );
					else {
						int x_offset = ( xi < 0 ) ? ( w + xi ) : (
							( xi > w - 1 ) ? ( xi - w ) : xi );
						int y_offset = ( yi < 0 ) ? ( h + yi ) : (
							( yi > h - 1 ) ? ( yi - h ) : yi );
						sum += GetBufferPixel( x_offset, y_offset );
					}

			SetBufferPixel( x, y, sum / ( r * r ) );
		}
}

ColorFloat operator+( const ColorFloat & lhs, const ColorFloat & rhs )
{
	ColorFloat out = lhs;
	out.b += rhs.b;
	out.g += rhs.g;
	out.r += rhs.r;
	return out;
}

ColorFloat operator*( const ColorFloat & lhs, const ColorFloat & rhs )
{
	ColorFloat out = lhs;
	out.b *= rhs.b;
	out.g *= rhs.g;
	out.r *= rhs.r;
	return out;
}

ColorFloat operator*( const ColorFloat & lhs, real t )
{
	ColorFloat out = lhs;
	out.b *= t;
	out.g *= t;
	out.r *= t;
	return out;
}

ColorFloat operator/( const ColorFloat & lhs, real t )
{
	ColorFloat out = lhs;
	out.b /= t;
	out.g /= t;
	out.r /= t;
	return out;
}

ColorFloat & ColorFloat::operator+=( const ColorFloat & rhs )
{
	b += rhs.b;
	g += rhs.g;
	r += rhs.r;

	return *this;
}

}