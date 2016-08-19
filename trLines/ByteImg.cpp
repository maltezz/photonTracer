#include "stdafx.h"
#include "ByteImg.h"

namespace app {

ByteImg::ByteImg( int width, int height )
	: image( width, height, PixelFormat24bppRGB )
{
	image.LockBits( &Gdiplus::Rect( 0, 0, width, height ), Gdiplus::ImageLockModeWrite, PixelFormat24bppRGB, &image_data );
	raw_data = static_cast< BYTE * >( image_data.Scan0 );
}

ByteImg::ByteImg( int width, int height, std::vector<BYTE> data )
	:image(width, height, 3 * width, PixelFormat24bppRGB, &data[0] )
{
	image.LockBits( &Gdiplus::Rect( 0, 0, width, height ), Gdiplus::ImageLockModeWrite, PixelFormat24bppRGB, &image_data );
	raw_data = static_cast< BYTE * >( image_data.Scan0 );
}

void ByteImg::SaveImage( const WCHAR * filename )
{
	image.UnlockBits( &image_data );
	// TODO: remove hardcoded fragment from GetEncoderClsid
	CLSID pClsid;
	GetEncoderClsid( L"image/bmp", &pClsid );
	image.Save( filename, &pClsid, NULL );
}

void ByteImg::SetPixel( int x, int y, const Color3 & c )
{
	int ind = GetPixelIndex( x, y );
	raw_data[ind] = c.r;
	raw_data[ind+1] = c.g;
	raw_data[ind+2] = c.b;
}

void ByteImg::SetPixel( int x, int y, const ColorFloat & c )
{
	int ind = GetPixelIndex( x, y );
	raw_data[ind] = (BYTE)(c.r * 255.0f);
	raw_data[ind + 1] = ( BYTE )( c.g * 255.0f );
	raw_data[ind + 2] = ( BYTE )( c.b * 255.0f );
}

Color3 ByteImg::GetPixel( int x, int y ) const
{
	int ind = GetPixelIndex( x, y );
	return{ raw_data[ind], raw_data[ind+1], raw_data[ind+2] };
}

int ByteImg::GetPixelIndex( int x, int y ) const
{
	return y * image_data.Stride + x * IMAGE_BPP;
}

int ByteImg::GetEncoderClsid( const WCHAR* format, CLSID* pClsid )
{
	using namespace::Gdiplus;
	UINT  num = 0;          // number of image encoders
	UINT  size = 0;         // size of the image encoder array in bytes

	ImageCodecInfo* pImageCodecInfo = NULL;

	GetImageEncodersSize( &num, &size );
	if ( size == 0 )
		return -1;  // Failure

	pImageCodecInfo = ( ImageCodecInfo* )( malloc( size ) );
	if ( pImageCodecInfo == NULL )
		return -1;  // Failure

	GetImageEncoders( num, size, pImageCodecInfo );

	for ( UINT j = 0; j < num; ++j )
	{
		if ( wcscmp( pImageCodecInfo[j].MimeType, format ) == 0 )
		{
			*pClsid = pImageCodecInfo[j].Clsid;
			free( pImageCodecInfo );
			return j;  // Success
		}
	}

	free( pImageCodecInfo );
	return -1;  // Failure
}

}