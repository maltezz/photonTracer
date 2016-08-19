// trLines.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "trLines.h"
#include "App.h"
#include "Point.h"
#include "LineSeg.h"
#include "BresenhamAlg.h"

std::unique_ptr<app::App> main_app;

int wmain( int argc, wchar_t *argv[] )
{
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	Gdiplus::GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, NULL );

	app::InitStruct params;
	wchar_t file_path[MAX_PATH];
	std::wcin.getline( file_path, MAX_PATH );
	params.level_file = file_path;
	std::wcin.getline( file_path, MAX_PATH );
	params.ocl_level_file = file_path;
	std::wcin.getline( file_path, MAX_PATH );
	params.out_level_file = file_path;
	std::wcin >> params.light_x >> params.light_y;
	std::wcin >> params.light_color.r >> params.light_color.g >> params.light_color.b;
	std::wcin >> params.num_rays;
	std::wcin >> params.filter;
	std::wcin >> params.exposure;
	std::wcin >> params.white;
	
	main_app = std::make_unique<app::App>();
	std::shared_ptr<app::BresenhamAlg> alg = std::make_shared<app::BresenhamAlg>();
	main_app->Initialize( params, alg );
	main_app->RayTracing();

	std::wcout << "DONE!" << std::endl;

	main_app.release();
	Gdiplus::GdiplusShutdown( gdiplusToken );

	return 0;
}
