#include "stdafx.h"
#include "Rectangle.h"
#include "App.h"
#include "BresenhamAlg.h"

namespace app {

void App::Initialize( InitStruct params_, std::shared_ptr<LineStrategy> alg )
{
	line_alg = alg;
	params = params_;

	float_image = std::make_unique<FloatImg>( params_.level_file, params_.ocl_level_file );

	float_image->SetExposure( params_.exposure );
	float_image->SetWhite( params_.white );
	float_image->SetFilterRadius( params_.filter );

	UINT w = float_image->GetWidth();
	UINT h = float_image->GetHeight();

	hypotenuse = ( real )sqrt( w * w + h * h );
	// 0 - left, 1 - right, 2 - top, 3 - bottom
	borders[0] = LineSeg( { 0, 0 }, { 0, ( real )( h - 1 ) } );
	borders[1] = LineSeg( { ( real )( w - 1 ), 0 }, { ( real )( w - 1 ), ( real )( w - 1 ) } );
	borders[2] = LineSeg( { 0, 0 }, { ( real )( w - 1 ), 0 } );
	borders[3] = LineSeg( { 0, ( real )( h - 1 ) }, { ( real )( w - 1 ), ( real )( h - 1 ) } );
}

void App::SaveImage()
{
	std::vector<BYTE> tmp_out = float_image->ConvertToBytes();
	Gdiplus::Bitmap out_img( float_image->GetWidth(), float_image->GetHeight(), 3 * float_image->GetWidth(), PixelFormat24bppRGB, &tmp_out[0] );
	size_t ext_pos = params.out_level_file.find_last_of( '.' ) + 1;

	if ( ext_pos == params.out_level_file.size() )
		return;

	std::wstring format = std::wstring( L"image/" ) + params.out_level_file.substr( ext_pos );
	CLSID pClsid;
	GetEncoderClsid( format.c_str(), &pClsid );
	out_img.Save( params.out_level_file.c_str(), &pClsid );
}

void App::RayTracing()
{
	real angle = 2.0 * std::_Pi;
	
	ColorFloat lc_f = params.light_color;
	Point center( params.light_x, params.light_y );

	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();
	rand_engine.seed( seed );

	for ( UINT i = 0; i < params.num_rays; ++i ) {
		real r = u_rand( rand_engine );

		Point trace_endp( hypotenuse * cos( angle * r ), hypotenuse * sin( angle * r ) );
		LineSeg trace( center, center + trace_endp );
		GenerateTraceLine( trace );

		TraceLine( trace, lc_f );

	}

	SaveImage();
}

Point App::Reflect( const Point & ray, const Point & normal )
{
	Point ray_inv = -ray;
	Point refl = normal * 2.0f * Point::dot( ray_inv, normal ) - ray_inv;
	refl.Normalize();
	return refl;
}

void App::GenerateTraceLine( LineSeg & ray ) const
{
	Point endp;
	for ( auto & test : borders ) {
		real t_;
		if ( LineSeg::isIntersect( test, ray, endp, t_ ) )
			break;
	}
	ray.p2 = endp - ray.p1;
}

LineSeg App::GenerateTraceLine( const Point & p1, const Point & p2 ) const
{
	LineSeg out( p1, p2 );
	GenerateTraceLine( out );
	return std::move( out );
}

void App::TraceLine( LineSeg ray, ColorFloat color )
{
	int iteration = 10000;
	while ( iteration > 0 ) {
		bool result = line_alg->DrawLine( *float_image, ray, color );
		// линия пересекается с геометрией
		if ( result ) {
			// generate random point
			Point r_point = { float_image->GetWidth() * u_rand( rand_engine ), float_image->GetHeight() * u_rand( rand_engine ) };
			r_point = r_point - ray.p1;
			r_point.Normalize();
			ray = GenerateTraceLine( ray.p1, ray.p1 + r_point * hypotenuse );
			--iteration;
			continue;
		}
		break;
	}
}

int App::GetEncoderClsid( const WCHAR* format, CLSID* pClsid )
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