// trLines.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "trLines.h"
#include "App.h"
#include "TestLine.h"
#include "Point.h"
#include "LineSeg.h"
#include "BresenhamAlg.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hWnd;
std::unique_ptr<app::App> main_app;
int px, py;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	Gdiplus::GdiplusStartup( &gdiplusToken, &gdiplusStartupInput, NULL );

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_TRLINES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

	std::shared_ptr<app::BresenhamAlg> alg = std::make_shared<app::BresenhamAlg>();
	main_app = std::make_unique<app::App>( 512, 480, alg );

/*	SCENE 1
	std::shared_ptr<app::TestLine> line = std::make_shared<app::TestLine>( 10.0f, 500.0f, 400.0f, 600.0f, alg );
	line->SetColor( { 255, 0, 0 } );
	main_app->AddShape( std::move( line ) );
	line = std::make_shared<app::TestLine>( 60.0f, 160.0f, 100.0f, 100.0f, alg );
	line->SetColor( { 0, 255, 0 } );
	main_app->AddShape( std::move( line ) );
	line = std::make_shared<app::TestLine>( 400.0f, 500.0f, 600.0f, 400.0f, alg );
	line->SetColor( { 255, 255, 0 } );
	main_app->AddShape( std::move( line ) );
	*/

	// BOX
	std::shared_ptr<app::TestLine> line = std::make_shared<app::TestLine>( 200.0f, 200.0f, 200.0f, 600.0f, alg );
	line->SetColor( { 0, 255, 0 } );
	main_app->AddShape( std::move( line ) );
	line = std::make_shared<app::TestLine>( 500.0f, 200.0f, 500.0f, 600.0f, alg );
	line->SetColor( { 0, 255, 0 } );
//	main_app->AddShape( std::move( line ) );
//	line = std::make_shared<app::TestLine>( 400.0f, 500.0f, 600.0f, 400.0f, alg );
//	line->SetColor( { 0, 255, 255 } );
//	main_app->AddShape( std::move( line ) );
	
//	main_app->SetClipRegion( Gdiplus::Rect( 0, 0, 700, 700 ) );

//	app::FloatImg img1( L"D:\\source\\Ò.Ò\\level.bmp", L"D:\\source\\Ò.Ò\\ocl.bmp" );

	main_app->RayTracing( app::Point( 50, 50 ), 500000 );
/*
    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRLINES));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
	*/
	main_app.release();
	Gdiplus::GdiplusShutdown( gdiplusToken );

	return 0;//( int )msg.wParam;
}

void DrawLine2( HDC & hdc, bool steep, int x0, int y0, int x1, int y1 );

void DrawLine( HDC & hdc, int x0, int y0, int x1, int y1){
	//	float total_len = sqrtf( ( x0 - x1 ) * ( x0 - x1 ) + ( y0 - y1 ) * ( y0 - y1 ) );
	bool steep = false;
	if ( std::abs( x1 - x0 ) < std::abs( y1 - y0 ) ) {
		std::swap( y0, x0 );
		std::swap( y1, x1 );
		steep = true;
	}
	bool reverse = false;
	if ( x0 > x1 ) {
	//	std::swap( y0, y1 );
	//	std::swap( x0, x1 );
		DrawLine2( hdc, steep, x0, y0, x1, y1 );
		return;
		reverse = true;
	}
	int error = 0;
	int y = y0;
	int dy = y1 - y0;
	int dx = x1 - x0;
	int sign = ( dx < 0 ) ? -1 : 1;
	int slope = ( dy * dx < 0 ) ? -1 : 1;
	dx = std::abs( dx );

	int old_x0 = x0;
	int old_y0 = y0;

	bool processed = false;

	for ( int x = x0; x <= x1; ++x ) {
		if ( steep ) {
			SetPixel( hdc, y, x, RGB( 0, 0, 0 ) );
		}
		else {
			SetPixel( hdc, x, y, RGB( 0, 0, 0 ) );
		}

		error += dy * sign;
		if ( std::abs( error << 1 ) >= dx ) {
			y += slope;
			error += -slope * dx;
		}
	}
}

// x1 < x0
void DrawLine2( HDC & hdc, bool steep, int x0, int y0, int x1, int y1 ) {

	int error = 0;
	int y = y0;
	int dy = y1 - y0;
	int dx = x1 - x0;
	int sign = ( dx > 0 ) ? -1 : 1;
	int slope = ( dy * dx > 0 ) ? -1 : 1;
	dx = std::abs( dx );

	int old_x0 = x0;
	int old_y0 = y0;

	bool processed = false;

	for ( int x = x0; x >= x1; --x ) {
		if ( steep ) {
			SetPixel( hdc, y, x, RGB( 0, 0, 0 ) );
		}
		else {
			SetPixel( hdc, x, y, RGB( 0, 0, 0 ) );
		}

		error += dy * sign;
		if ( std::abs( error << 1 ) >= dx ) {
			y += slope;
			error += -slope * dx;
		}
	}
}
//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRLINES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName = NULL;//MAKEINTRESOURCEW(IDC_TRLINES);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

int GetEncoderClsid( const WCHAR* format, CLSID* pClsid )
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

int get( int x, int y, int stride ) {
	return y * stride + 3 * x;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
	case WM_MOUSEMOVE:
		px = GET_X_LPARAM( lParam );
		py = GET_Y_LPARAM( lParam );
	//	InvalidateRect( hWnd, nullptr, TRUE );
		break;
	case WM_LBUTTONDOWN: {
		Gdiplus::Bitmap bb( 700, 700, PixelFormat24bppRGB );
	//	Gdiplus::Graphics g(
//		g.Clear( Gdiplus::Color( 0, 0, 0 ) );
	//	main_app->Draw( g );
//		main_app->RayTracing( g, app::Point( 350, 350 ), 10000 );
//		main_app->SaveImage( L"D:\\source\\image2.bmp" );
	//	alg.DrawLine( bb, 160, 40, 0, 0, Gdiplus::Color( 255, 255, 255 ), nullptr );
	//	CLSID pngClsid;
	//	GetEncoderClsid( L"image/bmp", &pngClsid );
	//	Gdiplus::Status gg = bb.Save( L"D:\\source\\image2.bmp", &pngClsid, NULL );
		MessageBeep( MB_OK );
	}break;
	case WM_RBUTTONDOWN: {
	/*	std::vector<BYTE> data;

	
		Gdiplus::Bitmap bb( L"D:\\source\\Ò.Ò\\level.bmp" ); // 500, 500, 3 * 500, PixelFormat24bppRGB, &data[0] );
		Gdiplus::PixelFormat wdwdw = bb.GetPixelFormat();
		int dww = PixelFormat24bppRGB;
		if ( PixelFormat24bppRGB == wdwdw ) {

		}
		Gdiplus::BitmapData data2;
		bb.LockBits( &Gdiplus::Rect( 0, 0, bb.GetWidth(), bb.GetHeight() ), Gdiplus::ImageLockModeWrite, PixelFormat24bppRGB, &data2 );
		BYTE * gg1 = ( BYTE * )data2.Scan0;
		data.resize( 3 * bb.GetWidth() * bb.GetHeight() );

		for ( int i = 0; i < data.size(); ++i )
			data[i] = gg1[i];

		for ( int i = 0; i < data.size() ; ++i )
			data[i] = 255;

		for ( int i = 0; i < data.size(); ++i )
			gg1[i] = data[i];

		bb.UnlockBits( &data2 );

		CLSID pngClsid;
		GetEncoderClsid( L"image/bmp", &pngClsid );
		Gdiplus::Status gg = bb.Save( L"D:\\source\\image3.bmp", &pngClsid, NULL );
	//	delete[] data;
	*/
		}break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			MoveToEx( hdc, 400, 400, nullptr );
			LineTo( hdc, 20, 100 );
			DrawLine( hdc, 400, 400, 20, 800 );
			DrawLine( hdc, 400, 400, 600, 600 );
		//	Gdiplus::Graphics g()
//			main_app->Draw( g );
		//	main_app->RayTracing( g, app::Point( 50, 50 ), 16 );
		
			

	/*		app::Point pt( 43, 00 );
			app::Point pt2( 50, 100 );
			app::LineSeg line1( pt, pt2 );

			Gdiplus::Pen p( Gdiplus::Color( 0, 0, 0 ) );
			
			g.DrawLine( &p, pt.x, pt.y, pt2.x, pt2.y );
			app::LineSeg line2( app::Point(0, 0), app::Point(px, py) );
			app::Point inter;
			if ( app::LineSeg::isIntersect( line1, line2, inter ) ) {
				g.DrawLine( &p, 0.0f, 0.0f, inter.x, inter.y );
			}else
				g.DrawLine( &p, 0, 0, px, py );
				*/
	/*		// Create a Rect object.
			Gdiplus::Graphics graphics( hdc );
			Gdiplus::Rect clipRect( 0, 0, 200, 100 );

			// Set the clipping region with hRegion.
		//	graphics.SetClip( clipRect );

			// Fill a rectangle to demonstrate the clipping region.
			graphics.FillRectangle( &Gdiplus::SolidBrush( Gdiplus::Color( 255, 0, 0, 0 ) ), 0, 0, 500, 500 );
			*/
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}
