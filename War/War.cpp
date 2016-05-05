#include "War.h"
#include "Error.h"
#include <sstream>

// Include libraries via source pragma. Better here than make files as it keeps header 
// and lib specification in the same place.
#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dxgi.lib" )
#pragma comment( lib, "dwrite.lib" )
#pragma comment( lib, "winmm.lib" )

// Implement a couple of operators for geometry primitives to simplify code.

D2D1_RECT_F operator+( const D2D1_RECT_F & lvalue, const D2D1_POINT_2F & rvalue )
{
	D2D1_RECT_F result =
	{ lvalue.left + rvalue.x, lvalue.top + rvalue.y, lvalue.right + rvalue.x, lvalue.bottom + rvalue.y };
	return result;
}

D2D1_POINT_2F operator*( const D2D1_POINT_2F & lvalue, const float & rvalue )
{
	return D2D1::Point2F( lvalue.x * rvalue, lvalue.y * rvalue );
}


WarApp::WarApp( HINSTANCE instance, char * cmdLine, int cmdShow, WNDPROC wndProc )
: m_mainHwnd( 0 )
, m_secondsPerCount( 0 )
, m_lastCount( 0 )
, m_elapsed( 0.0f )
, m_warMode( false )
{
	HRESULT hr = S_OK;

	{ // Enumerate displays.
		IDXGIFactory * factory = 0;
		CreateDXGIFactory( __uuidof( IDXGIFactory ), (void**)&factory );

		IDXGIAdapter * adapter = 0;
		for( int adapterIndex = 0; factory->EnumAdapters( adapterIndex, &adapter ) != DXGI_ERROR_NOT_FOUND; ++adapterIndex )
		{
			IDXGIOutput * output = 0;
			for( int outputIndex = 0; adapter->EnumOutputs( outputIndex, &output ) != DXGI_ERROR_NOT_FOUND; ++outputIndex )
			{
				DXGI_OUTPUT_DESC outputDesc = {};
				output->GetDesc( &outputDesc );
				if ( outputDesc.AttachedToDesktop )
				{
					HWND trayHandle = FindWindow( TEXT("Shell_TrayWnd"), 0 );
					RECT trayRect;
					GetWindowRect( trayHandle, &trayRect );
					// Shell_TrayWnd
					//SystemParametersInfo( SPI_GETWORKAREA, );
					MONITORINFO monitorInfo;
					monitorInfo.cbSize = sizeof( monitorInfo );
					GetMonitorInfo( outputDesc.Monitor, &monitorInfo );
					RECT rect = outputDesc.DesktopCoordinates;

					if ( trayRect.left == rect.left && trayRect.right == rect.right )
					{
						if ( trayRect.bottom == rect.bottom )
						{
							rect.bottom = trayRect.top;
						}
						else if ( trayRect.top == rect.top )
						{
							rect.top = trayRect.bottom;
						}
					}
					else if ( trayRect.top == rect.top && trayRect.bottom == rect.bottom )
					{
						if ( trayRect.left == rect.left )
						{
							rect.left = trayRect.right;
						}
						else if ( trayRect.right == rect.right )
						{
							rect.right = trayRect.left;
						}
					}

					m_displays.push_back( rect );
				}
			}
		}
	}
	
	hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_direct2DFactory );
	OnFailedThrow( hr, "Failed to create the Direct 2D Factory!" );

	hr = DWriteCreateFactory( DWRITE_FACTORY_TYPE_SHARED, __uuidof( IDWriteFactory ), reinterpret_cast< IUnknown** >( &m_directWriteFactory ) );
	OnFailedThrow( hr, "Failed to create DirectWrite factory!" );

	hr = m_directWriteFactory->CreateTextFormat( L"Arial", 0, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 10.0f * 96.0f / 72.0f, L"en-us", &m_textFormat );
	OnFailedThrow( hr, "Failed to create Text Format!" );

	// Create each display
	WNDCLASSEX wndClass = {};
	wndClass.cbSize = sizeof( WNDCLASSEX );
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = wndProc;
	wndClass.hInstance = instance;
	wndClass.lpszClassName = TEXT("HookWindow");
	if ( ! RegisterClassEx( &wndClass ) )
	{
		throw std::exception( "Failed to register window class!" );
	}
	DWORD exStyle = WS_EX_LAYERED;
	DWORD style = WS_VISIBLE;
	for( size_t i = 0, iend = m_displays.size(); i != iend; ++i )
	{
		Display & display = m_displays[ i ];

		HWND hwnd = CreateWindowEx( exStyle, TEXT("HookWindow"), TEXT("HookWindow"), style, display.GetRect().left, display.GetRect().top, display.GetWidth(), display.GetHeight(), m_mainHwnd, 0, instance, 0 );
		if ( ! m_mainHwnd )
		{
			m_mainHwnd = hwnd;
		}
		display.Create( hwnd, m_direct2DFactory );
	}

	CreateDeviceDependentResources();

	{ // Setup the application timer.
		long long frequency = 0;
		bool result = QueryPerformanceFrequency( (LARGE_INTEGER*)&frequency ) != 0;
		OnFalseThrow( result, "High performance timer not available!" );
			
		m_secondsPerCount = 1.0f / frequency;
		QueryPerformanceCounter( (LARGE_INTEGER*)&m_lastCount ); // Baseline.
	}
}

WarApp::~WarApp()
{
}

void WarApp::CreateDeviceDependentResources()
{
	HRESULT hr = S_OK;

	for ( size_t i = 0, iend = m_displays.size(); i != iend; ++i )
	{
		Display & display = m_displays[ i ];

		display.SetClearColor( D2D1::ColorF( D2D1::ColorF::Blue, 0.50f ) );

		ID2D1RenderTarget * target = display.GetTarget();
		if ( ! target )
		{
			display.CreateDeviceDependantResources( m_direct2DFactory );
			target = display.GetTarget();
		}
	
		CComPtr< ID2D1SolidColorBrush > brush;
		hr = target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::Azure ), &brush );
		OnFailedThrow( hr, "Failed to create render brush!" );

		CComPtr< ID2D1SolidColorBrush > fillBrush;
		hr = target->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::Blue, 0.25f ), &fillBrush );

		// Create the solid stroke.
		CComPtr< ID2D1StrokeStyle > solidStroke;
		hr = m_direct2DFactory->CreateStrokeStyle( D2D1::StrokeStyleProperties( D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_ROUND, D2D1_LINE_JOIN_MITER, 10.0f, D2D1_DASH_STYLE_SOLID, 0.0f ), 0, 0, &solidStroke );
		OnFailedThrow( hr, "Failed to create render stroke style!" );

		// Create the dashed stroke.
		CComPtr< ID2D1StrokeStyle > dashedStroke;
		hr = m_direct2DFactory->CreateStrokeStyle( D2D1::StrokeStyleProperties( D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_FLAT, D2D1_CAP_STYLE_ROUND, D2D1_LINE_JOIN_MITER, 10.0f, D2D1_DASH_STYLE_DASH, 0.0f ), 0, 0, &dashedStroke );
		OnFailedThrow( hr, "Failed to create render stroke style!" );

		{ // Window icons.
			CComPtr< ID2D1PathGeometry > geometry;
			hr = m_direct2DFactory->CreatePathGeometry( &geometry );
			OnFailedThrow( hr, "Failed to create Window Icon geometry!" );

			CComPtr< ID2D1GeometrySink > sink;
			geometry->Open( &sink );
			OnFailedThrow( hr, "Unable to open geometry!" );

			float scalar = 30.0f;
			sink->BeginFigure( D2D1::Point2F( 0.0f, 0.1f ) * scalar, D2D1_FIGURE_BEGIN_HOLLOW );
			sink->AddLine( D2D1::Point2F( 1.0f, 0.1f ) * scalar );
			sink->EndFigure( D2D1_FIGURE_END_OPEN );
			sink->BeginFigure( D2D1::Point2F( 0.0f, 0.0f ) * scalar, D2D1_FIGURE_BEGIN_FILLED );
			sink->AddLine( D2D1::Point2F( 0.0f, 0.0f ) * scalar );
			sink->AddLine( D2D1::Point2F( 1.0f, 0.0f ) * scalar );
			sink->AddLine( D2D1::Point2F( 1.0f, 1.0f ) * scalar );
			sink->AddLine( D2D1::Point2F( 0.0f, 1.0f ) * scalar );
			sink->EndFigure( D2D1_FIGURE_END_CLOSED );
			sink->Close();

			{ // Top half of display.
				Graphic * graphic = new Graphic( geometry, brush, 2.0f, solidStroke );
				D2D1_POINT_2F offset = D2D1::Point2F( display.GetWidth() * 0.5f - 0.5f * scalar, 20.0f );
				graphic->CombileTransformWith( D2D1::Matrix3x2F::Translation( offset.x, offset.y ) );
				graphic->SetFillBrush( fillBrush );

				ActiveArea area = 
				{
					D2D1::RectF( offset.x, offset.y, offset.x + scalar * 1.0f, offset.y + scalar * 1.0f ) + D2D1::Point2F( display.GetRectF().left, display.GetRectF().top ),
					{ display.GetRect().left, display.GetRect().top, display.GetWidth(), display.GetHeight()/ 2 }
				};
				m_activeAreas.push_back( area );
				display.GetRootGraphic()->AppendChild( "windowicontop", graphic );
			}
			{ // Bottom half of display.
				Graphic * graphic = new Graphic( geometry, brush, 2.0f, solidStroke );
				D2D1_POINT_2F offset = D2D1::Point2F( display.GetWidth() * 0.5f - 0.5f * scalar, display.GetHeight() - 20.0f - 1.0f * scalar );
				graphic->CombileTransformWith( D2D1::Matrix3x2F::Translation( offset.x, offset.y ) );
				graphic->SetFillBrush( fillBrush );
			
				ActiveArea area = 
				{
					D2D1::RectF( offset.x, offset.y, offset.x + scalar * 1.0f, offset.y + scalar * 1.0f ) + D2D1::Point2F( display.GetRectF().left, display.GetRectF().top ),
					{ display.GetRect().left, display.GetRect().top + display.GetHeight() / 2 + 1, display.GetWidth(), display.GetHeight() / 2 }
				};
				m_activeAreas.push_back( area );
				display.GetRootGraphic()->AppendChild( "windowiconbottom", graphic );
			}
			{ // Left half of display.
				Graphic * graphic = new Graphic( geometry, brush, 2.0f, solidStroke );
				D2D1_POINT_2F offset = D2D1::Point2F( 20.0f, display.GetHeight() * 0.5f - 0.5f * scalar );
				graphic->CombileTransformWith( D2D1::Matrix3x2F::Translation( offset.x, offset.y ) );
				graphic->SetFillBrush( fillBrush );
			
				ActiveArea area = 
				{
					D2D1::RectF( offset.x, offset.y, offset.x + scalar * 1.0f, offset.y + scalar * 1.0f ) + D2D1::Point2F( display.GetRectF().left, display.GetRectF().top ),
					{ display.GetRect().left, display.GetRect().top, display.GetWidth() / 2, display.GetHeight() }
				};
				m_activeAreas.push_back( area );
				display.GetRootGraphic()->AppendChild( "windowiconleft", graphic );
			}
			{ // Right half of display.
				Graphic * graphic = new Graphic( geometry, brush, 2.0f, solidStroke );
				D2D1_POINT_2F offset = D2D1::Point2F( display.GetWidth() - 20.0f - 1.0f * scalar, display.GetHeight() * 0.5f - 0.5f * scalar );
				graphic->CombileTransformWith( D2D1::Matrix3x2F::Translation( offset.x, offset.y ) );
				graphic->SetFillBrush( fillBrush );
			
				ActiveArea area = 
				{
					D2D1::RectF( offset.x, offset.y, offset.x + scalar * 1.0f, offset.y + scalar * 1.0f ) + D2D1::Point2F( display.GetRectF().left, display.GetRectF().top ),
					{ display.GetRect().left + display.GetWidth() / 2, display.GetRect().top, display.GetWidth() / 2, display.GetHeight() }
				};
				m_activeAreas.push_back( area );
				display.GetRootGraphic()->AppendChild( "windowiconright", graphic );
			}


			{ // Top-left quarter of display.
				Graphic * graphic = new Graphic( geometry, brush, 2.0f, solidStroke );
				D2D1_POINT_2F offset = D2D1::Point2F( 20.0f, 20.0f );
				graphic->CombileTransformWith( D2D1::Matrix3x2F::Translation( offset.x, offset.y ) );
				graphic->SetFillBrush( fillBrush );

				ActiveArea area = 
				{
					D2D1::RectF( offset.x, offset.y, offset.x + scalar * 1.0f, offset.y + scalar * 1.0f ) + D2D1::Point2F( display.GetRectF().left, display.GetRectF().top ),
					{ display.GetRect().left, display.GetRect().top, display.GetWidth() / 2, display.GetHeight() / 2 }
				};
				m_activeAreas.push_back( area );
				display.GetRootGraphic()->AppendChild( "windowicontop", graphic );
			}		
			{ // Top-right quarter of display.
				Graphic * graphic = new Graphic( geometry, brush, 2.0f, solidStroke );
				D2D1_POINT_2F offset = D2D1::Point2F( display.GetWidth() - 20.0f - 1.0f * scalar, 20.0f );
				graphic->CombileTransformWith( D2D1::Matrix3x2F::Translation( offset.x, offset.y ) );
				graphic->SetFillBrush( fillBrush );

				ActiveArea area = 
				{
					D2D1::RectF( offset.x, offset.y, offset.x + scalar * 1.0f, offset.y + scalar * 1.0f ) + D2D1::Point2F( display.GetRectF().left, display.GetRectF().top ),
					{ display.GetRect().left + display.GetWidth() / 2, display.GetRect().top, display.GetWidth() / 2, display.GetHeight() / 2 }
				};
				m_activeAreas.push_back( area );
				display.GetRootGraphic()->AppendChild( "windowicontop", graphic );
			}		
			{ // Bottom-left quarter of display.
				Graphic * graphic = new Graphic( geometry, brush, 2.0f, solidStroke );
				D2D1_POINT_2F offset = D2D1::Point2F( 20.0f, display.GetHeight() - 20.0f - 1.0f * scalar );
				graphic->CombileTransformWith( D2D1::Matrix3x2F::Translation( offset.x, offset.y ) );
				graphic->SetFillBrush( fillBrush );

				ActiveArea area = 
				{
					D2D1::RectF( offset.x, offset.y, offset.x + scalar * 1.0f, offset.y + scalar * 1.0f ) + D2D1::Point2F( display.GetRectF().left, display.GetRectF().top ),
					{ display.GetRect().left, display.GetRect().top + display.GetHeight() / 2 + 1, display.GetWidth() / 2, display.GetHeight() / 2 }
				};
				m_activeAreas.push_back( area );
				display.GetRootGraphic()->AppendChild( "windowicontop", graphic );
			}		
			{ // Bottom-right quarter of display.
				Graphic * graphic = new Graphic( geometry, brush, 2.0f, solidStroke );
				D2D1_POINT_2F offset = D2D1::Point2F( display.GetWidth() - 20.0f - 1.0f * scalar, display.GetHeight() - 20.0f - 1.0f * scalar );
				graphic->CombileTransformWith( D2D1::Matrix3x2F::Translation( offset.x, offset.y ) );
				graphic->SetFillBrush( fillBrush );

				ActiveArea area = 
				{
					D2D1::RectF( offset.x, offset.y, offset.x + scalar * 1.0f, offset.y + scalar * 1.0f ) + D2D1::Point2F( display.GetRectF().left, display.GetRectF().top ),
					{ display.GetRect().left + display.GetWidth() / 2, display.GetRect().top + display.GetHeight() / 2 + 1, display.GetWidth() / 2, display.GetHeight() / 2 }
				};
				m_activeAreas.push_back( area );
				display.GetRootGraphic()->AppendChild( "windowicontop", graphic );
			}		
		}
	}
}

void WarApp::DiscardDeviceDependentResources()
{
}

void WarApp::Go()
{
	MSG msg = MSG();
	do
	{
		if ( PeekMessageW( &msg, 0, 0, 0, PM_REMOVE ) )
		{
			TranslateMessage( &msg );
			DispatchMessage( &msg );
		}

		Update();
		Render();

	} while( msg.message != WM_QUIT );
}

void WarApp::Update()
{
	{ // Update time delta.
		long long currentCount = 0;
		QueryPerformanceCounter( (LARGE_INTEGER*)&currentCount );
		m_elapsed = ( currentCount - m_lastCount ) * m_secondsPerCount;
		m_lastCount = currentCount;
	}

	// Determine if we are in war mode.
	HWND foregroundHwnd = GetForegroundWindow();
	bool previousWarMode = m_warMode;
	POINT currentPosition = {};
	GetCursorPos( &currentPosition );
	if ( foregroundHwnd )
	{
		bool mine = false;
		{
			for ( size_t i = 0, iend = m_displays.size(); i != iend; ++i )
			{
				const Display & display = m_displays[ i ];
				if ( foregroundHwnd == display.GetHwnd() )
				{
					mine = true;
					break;
				}
			}
		}
		if ( ! mine )
		{
			RECT rect;
			GetWindowRect( foregroundHwnd, &rect );
			ScreenToClient( m_mainHwnd, (LPPOINT)&rect.left );
			ScreenToClient( m_mainHwnd, (LPPOINT)&rect.right );
			
			DWORD tid = 0;
			GUITHREADINFO guiThreadInfo = GUITHREADINFO();
			guiThreadInfo.cbSize = sizeof( GUITHREADINFO );
			tid = GetWindowThreadProcessId( foregroundHwnd, 0 );
			bool result;
			result = GetGUIThreadInfo( tid, &guiThreadInfo ) == 0 ? false : true;
			if ( result )
			{
				//OnFalseGetLastErrorThrow( result, " Failed to get GUI thread info!" );

				if ( guiThreadInfo.hwndMoveSize )
				{
					// Enter war mode if we aren't already in it.
					if ( ! m_warMode )
					{
						m_warMode = true;
						for( size_t i = 0, iend = m_displays.size(); i != iend; ++i )
						{
							Display & display = m_displays[ i ];
							ShowWindow( display.GetHwnd(), SW_SHOWNA );
						}
					}
					else // Already in war mode.
					{
					}
				}
				else
				{
					m_warMode = false;
				}
			}
		}
		else
		{
			m_warMode = false;
		}
	}
	else
	{
		m_warMode = false;
	}

	if ( previousWarMode && ! m_warMode ) 
	{
		for( size_t i = 0; i < m_activeAreas.size(); ++i )
		{
			const ActiveArea & activeArea = m_activeAreas[ i ];
			float x = static_cast< float >( currentPosition.x );
			float y = static_cast< float >( currentPosition.y );
			const D2D1_RECT_F & hoverArea = activeArea.hoverArea;

			if ( x >= hoverArea.left && y >= hoverArea.top && x <= hoverArea.right && y <= hoverArea.bottom )
			{
				const RECT & moveWindowArea = activeArea.moveWindowArea;
				bool result = MoveWindow( foregroundHwnd, moveWindowArea.left, moveWindowArea.top, moveWindowArea.right, moveWindowArea.bottom, true ) ? true : false;
				OnFalseThrow( result, "Failed to move window!" );
			}
		}
	}
}

void WarApp::Render()
{
	// Don't render if not in war mode.
	if ( ! m_warMode )
	{
		for( size_t i = 0, iend = m_displays.size(); i != iend; ++i )
		{
			m_displays[ i ].Hide();
		}
		return;
	}

	for( size_t i = 0, iend = m_displays.size(); i != iend; ++i )
	{
		m_displays[ i ].Draw();
	}
}
