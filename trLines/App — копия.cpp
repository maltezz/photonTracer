#include "stdafx.h"
#include "Rectangle.h"
#include "App.h"
#include "BresenhamAlg.h"

namespace app {

App::App( int width, int height, std::shared_ptr<LineStrategy> alg )
	:main_image( width, height )
{
	border.x = width;
	border.y = height;
	line_alg = alg;

	hypotenuse = sqrtf( border.x * border.x + border.y * border.y );

	// 0 - left, 1 - right, 2 - top, 3 - bottom
	borders[0] = LineSeg( { 0, 0 }, { 0, ( real )( height - 1 ) } );
	borders[1] = LineSeg( { ( real )( width - 1 ), 0 }, { ( real )( width - 1 ), ( real )( height - 1 ) } );
	borders[2] = LineSeg( { 0, 0 }, { ( real )( width - 1 ), 0 } );
	borders[3] = LineSeg( { 0, ( real )( height - 1 ) }, { ( real )( width - 1 ), ( real )( height - 1 ) } );
}

void App::Draw()
{
	for ( auto s : objects ) {
		s->Draw( main_image );
	}
}

void App::AddShape( std::shared_ptr<Object> object )
{
	objects.push_back( object );
}

void App::SaveImage( const WCHAR * filename )
{
	main_image.SaveImage( filename );
}

void app::App::LoadLevel( const WCHAR * filename, const WCHAR * ocl_fn )
{

}

void App::RayTracing( Point center, int count )
{
	static float pi = 3.14159265358979323846f;
	float angle = 2.0f * pi;

	FloatImg tmp_img( border.x, border.y );
	ColorFloat lc_f = { 0.6f, 0.6f, 0.6f };

	float_image = new FloatImg( L"D:\\source\\Т.Т\\level.bmp", L"D:\\source\\Т.Т\\ocl.bmp" );//new FloatImg( border.x, border.y );

	auto seed = std::chrono::high_resolution_clock::now().time_since_epoch().count();

	for ( int i = 0; i < count; ++i ) {
		float r = u_rand( rand_engine );
	//	Point trace_endp( hypotenuse * cosf( angle * i ), hypotenuse * sinf( angle * i ) );


		Point trace_endp( hypotenuse * cosf( angle * r ), hypotenuse * sinf( angle * r ) );
		LineSeg trace( center, center + trace_endp );
		GenerateTraceLine( trace );
		TraceLine( trace, lc_f, 0, 0, -1 );

		// Trace line

/*		std::weak_ptr<Object> min_obj; // closest object;
		IntersectData min_point;
		min_point.t = 10000.0f;

		for ( int i = 0; i < objects.size(); ++i ) {
			IntersectData idata;
			if ( objects[i]->IsIntersect( trace, idata ) ) {
				if ( idata.t < min_point.t ) {
					min_point = idata;
					min_obj = objects[i];
				}
			}
		}

		if ( !min_obj.expired() ) {
		//  DEBUG: draw normal
		//	LineSeg normal = GenerateTraceLine( min_point.p, min_point.p + min_point.normal * sz );
		//	line_alg->DrawLine( main_image, normal, lc_blue, nullptr );

			// Draw light line
		//	line_alg->DrawLine( main_image, ( int )center.x, ( int )center.y, ( int )(center.x + min_point.t * trace.p2.x), ( int )(center.y + min_point.t * trace.p2.y), lc );
			line_alg->DrawLine( tmp_img, ( int )center.x, ( int )center.y, ( int )( center.x + min_point.t * trace.p2.x ), ( int )( center.y + min_point.t * trace.p2.y ), lc_f, nullptr );

		//  DEBUG: draw reflect vector
			Point reflect = Reflect( trace.p2, min_point.normal );
			LineSeg refl_seg = GenerateTraceLine( min_point.p, min_point.p + reflect * sz );
		//	line_alg->DrawLine( main_image, refl_seg ,lc_red, nullptr );
		//	line_alg->DrawLine( tmp_img, refl_seg, lc_blue_f, nullptr );
		}else {
		//	line_alg->DrawLine( main_image, trace, lc );
			line_alg->DrawLine( tmp_img, trace, lc_f, nullptr );
		}
		*/
	}


	std::vector<BYTE> tmp_out = float_image->ConvertToBytes();
//	ByteImg new_img( border.x, border.y, ( tmp_out ) );
	Gdiplus::Bitmap out_img12( 512, 480, 3 * 512, PixelFormat24bppRGB, &tmp_out[0] );
	CLSID pClsid;
	ByteImg::GetEncoderClsid( L"image/bmp", &pClsid );
	out_img12.Save( L"D:\\source\\image3.bmp", &pClsid );

/*	for ( auto & obj : objects ) {
		obj->Draw( new_img );
	}
	*/
//	new_img.SaveImage( L"D:\\source\\image3.bmp" );

	delete float_image;
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
		float t_;
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

void App::TraceLine( LineSeg ray, ColorFloat color, int n, int max, int last )
{
	if ( n > max ) return;

	std::weak_ptr<Object> min_obj; // closest object;
	IntersectData min_point;
	min_point.t = 10000.0f;

	int iteration = 10000;
	while ( iteration > 0 ) {
		bool result = line_alg->DrawLine( *float_image, ray, color );
		if ( result ) {
			// generate random point
			Point r_point = { border.x * u_rand( rand_engine ), border.y * u_rand( rand_engine ) };
			r_point = r_point - ray.p1;
			r_point.Normalize();
			ray = GenerateTraceLine( ray.p1, ray.p1 + r_point * hypotenuse );
			--iteration;
			continue;
		//	line_alg->DrawLine( *float_image, s, color );
		}
		break;
	}

/*	for ( int i = 0; i < objects.size(); ++i ) {
		IntersectData idata;
		if ( i != last && objects[i]->IsIntersect( ray, idata ) ) {
			if ( idata.t < min_point.t ) {
				min_point = idata;
				min_obj = objects[i];
				last = i;
			}
		}
	}
	

	if ( !min_obj.expired() ) {
		//  DEBUG: draw normal
		//	LineSeg normal = GenerateTraceLine( min_point.p, min_point.p + min_point.normal * sz );
		//	line_alg->DrawLine( main_image, normal, lc_blue, nullptr );

		// Draw light line
		//	line_alg->DrawLine( main_image, ( int )center.x, ( int )center.y, ( int )(center.x + min_point.t * trace.p2.x), ( int )(center.y + min_point.t * trace.p2.y), lc );
		color = line_alg->DrawLine( *float_image, ( int )ray.p1.x, ( int )ray.p1.y, ( int )( ray.p1.x + min_point.t * ray.p2.x ), ( int )( ray.p1.y + min_point.t * ray.p2.y ), color, nullptr );

		//  DEBUG: draw reflect vector
		Point reflect = Reflect( ray.p2, min_point.normal );
		// не очень точно, но быстро
		Point in_v = -ray.p2;
		in_v.Normalize();
		float diff = Point::dot( in_v, reflect );
		LineSeg refl_seg = GenerateTraceLine( min_point.p, min_point.p + reflect * hypotenuse );
		//	line_alg->DrawLine( main_image, refl_seg ,lc_red, nullptr );
	//	Color3 ct = min_obj.lock()->GetColor();
		TraceLine( refl_seg, color * min_obj.lock()->GetFloatColor()  , n + 1, max, last );
		//	line_alg->DrawLine( *float_image, refl_seg, ( color + min_obj.lock()->GetFloatColor() ) * diff, nullptr );
	}
	else {
		//	line_alg->DrawLine( main_image, trace, lc );
	//	line_alg->DrawLine( *float_image, ray, color, nullptr );
		line_alg->DrawLine( *float_image, ( int )ray.p1.x, ( int )ray.p1.y, ( int )( ray.p1.x + ray.p2.x ), ( int )( ray.p1.y + ray.p2.y ), color, nullptr );
	}
	*/
}

}