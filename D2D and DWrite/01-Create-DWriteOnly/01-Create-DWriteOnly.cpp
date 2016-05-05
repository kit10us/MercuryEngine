/*
	* Inclusion

	Use of CComPtr (include atlbawse.h):
	Use of DirectX, Direct2D, et al, requires use of COM Objects. These objects are, for all intents a purposes, pointers. They thus require manual
	lifetime management (creation, validation, destruction). The CComPtr is a shared pointer specifically designed for COM Objects. It is similar
	to the shared_ptr (boost, tr1, std), except they are better suited for COM Objects and simplify the code more so than using the shared_ptr.
	Some places of simplification include address-of operator use, allowing us to directly use a CComPtr when requiring the passing of a COM pointer
	instead of having to create a temporary pointer, then placing that temp into a shared_ptr. This typically removes two extra lines of code and
	a variable declaration. Much cleaner.
	A CComPtr automatically cases to its underlying COM object, removing the need to use shared_ptr's get.
*/

#include <Windows.h>
#include <d2d1.h>		// Core D2D features.
#include <dxerr.h>		// For DXGetErrorStringA
#include <string>
#include <atlbase.h>	// For CComPtr


// Include libraries via source pragma. Better here than make files as it keeps header 
// and lib specification in the same place.
#pragma comment( lib, "d2d1.lib" )
#pragma comment( lib, "dxerr.lib" )

// Preprocessor define instead of function to prevent initialization of 'message'.
#define OnFailedThrow( hr, message ) \
	if ( FAILED( hr ) ) { \
		std::string what( message ); \
		what += std::string( "\n[" ) + DXGetErrorStringA( hr ) + std::string( "]" ); \
		throw std::exception( what.c_str() ); \
	}

#define OnFalseThrow( result, message ) \
	if ( ! ( result ) ) throw std::exception( std::string( message ).c_str() );

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam );

class Direct2DApp
{
	HWND m_hwnd;
	CComPtr< ID2D1Factory > m_direct2DFactory;
	CComPtr< ID2D1HwndRenderTarget > m_renderTarget;
	float m_secondsPerCount;
	long long m_lastCount;
	float m_elapsed;

	CComPtr< ID2D1SolidColorBrush > m_brush;

public:
	Direct2DApp();
	~Direct2DApp();
	void Startup( HINSTANCE instance, char * cmdLine, int cmdShow );
	void CreateDeviceDependentResources();
	void DiscardDeviceDependentResources();
	void Update();
	void Render();
};

Direct2DApp::Direct2DApp()
	: m_hwnd( 0 )
	, m_secondsPerCount( 0 )
	, m_lastCount( 0 )
	, m_elapsed( 0.0f )
{
}

Direct2DApp::~Direct2DApp()
{
}

void Direct2DApp::Startup( HINSTANCE instance, char * cmdLine, int cmdShow )
{
	int width = 800;
	int height = 600;
	HRESULT hr = S_OK;

	{ // Create Direct 2D indepdent resources.
		hr = D2D1CreateFactory( D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_direct2DFactory );
		OnFailedThrow( hr, "Failed to create the Direct 2D Factory!" );
	}

	{ // Create the window.
		WNDCLASSEX wndClass = WNDCLASSEX();
		wndClass.cbSize = sizeof( WNDCLASSEX );
		wndClass.style = CS_HREDRAW | CS_VREDRAW;
		wndClass.lpfnWndProc = WndProc;
		wndClass.hInstance = instance;
		wndClass.lpszClassName = TEXT("HookWindow");
		if ( ! RegisterClassEx( &wndClass ) )
		{
			throw std::exception( "Failed to register window class!" );
		}

		m_hwnd = CreateWindow( TEXT("HookWindow"), TEXT("HookWindow"), WS_OVERLAPPED, 10, 10, width, height, 0, 0, instance, 0 );
		UpdateWindow( m_hwnd );
		ShowWindow( m_hwnd, cmdShow );
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

void Direct2DApp::CreateDeviceDependentResources()
{
	HRESULT hr = S_OK;

	if ( ! m_renderTarget )
	{
		RECT rect;
		GetClientRect( m_hwnd, &rect );
		D2D1_SIZE_U size = D2D1::SizeU( rect.right - rect.left, rect.bottom - rect.top );

		hr = m_direct2DFactory->CreateHwndRenderTarget( D2D1::RenderTargetProperties(), D2D1::HwndRenderTargetProperties( m_hwnd, size ), &m_renderTarget );
		OnFailedThrow( hr, "Failed to create Direct 2D Render Target! ");

		hr = m_renderTarget->CreateSolidColorBrush( D2D1::ColorF( D2D1::ColorF::Azure ), &m_brush );
		OnFailedThrow( hr, "Failed to create render brush!" );
	}
}

void Direct2DApp::DiscardDeviceDependentResources()
{
	m_renderTarget = 0;
}

void Direct2DApp::Update()
{
	{ // Update time delta.
		long long currentCount = 0;
		QueryPerformanceCounter( (LARGE_INTEGER*)&currentCount );
		m_elapsed = ( currentCount - m_lastCount ) * m_secondsPerCount;
		m_lastCount = currentCount;
	}
}

void Direct2DApp::Render()
{
	CreateDeviceDependentResources();

	if ( m_renderTarget && m_renderTarget->CheckWindowState() == D2D1_WINDOW_STATE_NONE )
	{
		// Being drawing.
		m_renderTarget->BeginDraw();

		// Set our initial transformation.
		m_renderTarget->SetTransform( D2D1::Matrix3x2F::Identity() );

		// Clear.
		m_renderTarget->Clear( D2D1::ColorF( D2D1::ColorF::Red ) );

		// Draw a line.
		m_renderTarget->DrawLine( D2D1::Point2( 10.0f, 10.0f ), D2D1::Point2( 300.0f, 30.0f ), m_brush, 3.0f );

		// Attempt to end drawing, if failed, attempt a recreate of resources (this frame is then not rendered).
		// [http://msdn.microsoft.com/en-us/library/windows/desktop/dd371461%28v=vs.85%29.aspx]
		// "If the adapter a render target is on has been removed or the driver upgraded while the application is running, this is returned as an error in the EndDraw call."
		if ( m_renderTarget->EndDraw() == D2DERR_RECREATE_TARGET )
		{
			DiscardDeviceDependentResources();
		}
	}
}

LRESULT CALLBACK WndProc( HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch( msg )
	{
	case WM_KEYUP:
		switch( wParam )
		{
		case VK_ESCAPE:
			PostQuitMessage( 0 );
			return 1;
		}
		break;
	}
	return DefWindowProc( hwnd, msg, wParam, lParam );
}

int WINAPI WinMain( HINSTANCE instance, HINSTANCE prevInstance, char * cmdLine, int cmdShow )
{
	try
	{
		Direct2DApp app;

		app.Startup( instance, cmdLine, cmdShow );

		MSG msg = MSG();
		do
		{
			if ( PeekMessageW( &msg, 0, 0, 0, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}

			app.Update();
			app.Render();

		} while( msg.message != WM_QUIT );
	}
	catch( std::exception & ex )
	{
		std::string text = "Exception:\n" + std::string( ex.what() );
		MessageBoxA( 0, text.c_str(), "Error", MB_OK );
	}
	catch( ... )
	{
		MessageBoxA( 0, "Unknown error!", "Error", MB_OK );
	}

	return 0;
}

