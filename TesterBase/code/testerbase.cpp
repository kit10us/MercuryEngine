#include <TesterBase.h>
#include <Draw2D.h>
#include <assert.h>

#ifdef _DEBUG
# pragma comment( lib, "d3d9" )
# pragma comment( lib, "d3dx9" )
#else // NDEBUG
# pragma comment( lib, "d3d9" )
# pragma comment( lib, "d3dx9" )
#endif

#pragma comment( lib, "winmm" )



extern "C"
LRESULT CALLBACK WndProc( HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	return TesterBase::GetInstance()->HandleWndProc( hWnd, message, wParam, lParam );
}

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR cmdLine, int nCmdShow )
{
	hPrevInstance; // Not used
	int returnValue = 0;
	try
	{
		returnValue = TesterBase::GetInstance()->Go( hInstance, cmdLine , nCmdShow );
	}
	catch( const std::exception & exception )
	{
		MessageBoxA( 0, exception.what(), "Exception!", MB_ICONEXCLAMATION );
	}

	return returnValue;
}


TesterBase::TesterBase()
: m_hwnd( HWND() )
, m_isQuitting( false )
, m_hardwareAcceleration( true )
{
	if( s_tester )
	{
		throw std::exception( "Tester application already exists! There can be only one." );
	}
	s_tester = this;
}

TesterBase::~TesterBase()
{
    //DestroyDefaultShaders();
    
	s_tester = 0;
}

void TesterBase::SetHardwareAcceleration( bool enabled )
{
    m_hardwareAcceleration = enabled;
}

bool TesterBase::GetHardwareAcceleration() const
{
    return m_hardwareAcceleration;
}

int TesterBase::Go( HINSTANCE hInstance, LPSTR cmdLine, int cmdShow  )
{
    m_hInstance = hInstance;
    m_cmdShow = cmdShow;

	MSG msg = MSG(); // To remove warning C4701: potentially uninitialized local variable 'msg' used;
	try 
	{

		BeforeStartup( std::string( cmdLine ) );
		CreatePendingDisplays();

		for( auto & display : m_actualDisplays )
		{
			display.second.CreateDefaultShaders();
		}

		Startup();

		int forever = 1; // To remove warning C4127: conditional expression is constant
		while( forever )
		{ 
			if( PeekMessage( &msg, NULL, 0, 0, PM_REMOVE ) == 1 )
			{
				if( msg.message == WM_QUIT )
				{
					break;
				}

				// Determine if this message is for a child window...
				bool childHandledTheMessage = false;
				for ( std::list< HWND >::iterator itr = m_childHandles.begin(), end = m_childHandles.end(); itr != end && ! childHandledTheMessage; ++itr )
				{
					HWND & hwnd = *itr;
					childHandledTheMessage = IsDialogMessageA( hwnd, &msg ) ? true : false;
				}

				if ( ! childHandledTheMessage )
				{
					TranslateMessage( &msg );
					DispatchMessage( &msg );
				}
			}

			static unsigned int lastTime = timeGetTime();
			unsigned int currentTime = timeGetTime();
			unsigned int difference = (currentTime - lastTime);
			float elapsed = difference * 0.001f;
			lastTime = currentTime;

			Update( elapsed, m_input );
			CreatePendingDisplays();
			if( IsQuitting() )
			{
				PostQuitMessage( 0 );
				break;
			}
			m_input.CallAfterUpdate();

			// We render each display one at a time...
			int index = 0;
			for ( auto & actual : m_actualDisplays )
			{
				if ( actual.first.IsDialog() == false )
				{
					actual.second.BeforeRender();
					Render( actual.second.GetDxDevice(), index++ );
					actual.second.AfterRender();
				}
			}        
		} 
		Shutdown();
	}
	catch( std::exception & e )
	{
		MessageBoxA( 0, e.what(), "Error", MB_OK );
	}
	catch( ... )
	{
		MessageBoxA( 0, "Unknown expection!", "Error", MB_OK );
	}
	DestroyDirectX();
	DestroyWindow( m_hwnd );

    return static_cast< int >( msg.wParam );
}

LRESULT TesterBase::HandleWndProc( HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam )
{
	switch ( message )
	{
	case WM_DESTROY:
        return DefWindowProc( hwnd, message, wParam, lParam );
	case WM_CLOSE:
        m_isQuitting = true;
        DestroyWindow( hwnd );
        return 0;
	case WM_KEYDOWN:
		if( wParam == VK_ESCAPE )
		{
			TesterBase::s_tester->RequestQuit();
		}
		else
		{
			Input & input = TesterBase::s_tester->GetInput();
			input.SetKey( static_cast< unsigned int >( wParam ), true );
		}
		break;
	case WM_KEYUP:
		if( wParam == VK_ESCAPE )
		{
		}
		else
		{
			Input & input = TesterBase::s_tester->GetInput();
			input.SetKey( static_cast< unsigned int >( wParam ), false );
		}
		break;
	case WM_LBUTTONDOWN:
		{
			Input & input = TesterBase::s_tester->GetInput();
			input.SetLeftMouse( true );
		}
		break;
	case WM_LBUTTONUP:
		{
			Input & input = TesterBase::s_tester->GetInput();
			input.SetLeftMouse( false );
		}
		break;
	case WM_RBUTTONDOWN:
		{
			Input & input = TesterBase::s_tester->GetInput();
			input.SetRightMouse( true );
		}
		break;
	case WM_RBUTTONUP:
		{
			Input & input = TesterBase::s_tester->GetInput();
			input.SetRightMouse( false );
		}
		break;
	case WM_MOUSEMOVE:
		{
			size_t displayIndex = size_t();
			bool displayFound = GetDisplayByHWND( hwnd, displayIndex );
			assert( displayFound );

			Input & input = GetInput();
			
			RECT clientRect;
			GetClientRect( hwnd, &clientRect );

			const std::pair< DisplayDetails, DisplayActual > & display = GetDisplay( displayIndex );

			float width = static_cast< float >( display.first.GetSize().width );
			float height = static_cast< float >( display.first.GetSize().height );
			float clientWidth = static_cast< float >( clientRect.right );
			float clientHeight = static_cast< float >( clientRect.bottom );

			V2 mousePosition( static_cast< float >( LOWORD( lParam ) ), static_cast< float >( HIWORD( lParam ) ) );
			mousePosition.x *= width / clientWidth;
			mousePosition.y *= height / clientHeight;
			input.SetMousePosition( displayIndex, mousePosition );
		}
		break;
	}

	return DefWindowProc( hwnd, message, wParam, lParam );
}

// Note: BeforeStartup( ... ) is pure virtual at this level of inheritance.

void TesterBase::AddDisplay( const DisplayDetails & displayDetails )
{
    m_pendingDisplays.push_back( displayDetails );
}

void TesterBase::CreatePendingDisplays()
{
    if ( m_pendingDisplays.empty() )
    {
        return;
    }

    for ( std::vector< DisplayDetails >::const_iterator itr = m_pendingDisplays.begin(), end = m_pendingDisplays.end(); itr != end; ++itr )
    {
        const DisplayDetails & details = *itr;
        CreateDisplay( details );
    }

    m_pendingDisplays.clear();
}

void TesterBase::CreateDisplay( const DisplayDetails & details )
{
    bool isPrimary = m_actualDisplays.empty() ? true : false; // Note that this is VERY explicit - we are actual spelling out our intention (even though it looks redundant).

    DisplayActual actual = DisplayActual();

    if ( details.IsDialog() )
    {
        // Note: If we are a dialog, we do NOT have a Direct-X Display Device.
		HINSTANCE hInstance = GetHINSTANCE(); // [optional] A handle to the module whose executable file contains the dialog box template. 
		LPCSTR lpName = details.GetDialogTemplateName();
		
		//MAKEINTRESOURCEA( IDD_ENUM ); // The dialog box template. This parameter is either the pointer to a null-terminated character string that specifies the name of the dialog box template or an integer value that specifies the resource identifier of the dialog box template. If the parameter specifies a resource identifier, its high-order word must be zero and its low-order word must contain the identifier. You can use the MAKEINTRESOURCE macro to create this value. 
		HWND hWndParent = details.GetParentHandle(); // [optional] A handle to the window that owns the dialog box. 
		DLGPROC lpDialogFunc = details.GetDialogProc() ? details.GetDialogProc() : (DLGPROC)WndProc; // [optional] A pointer to the dialog box procedure. For more information about the dialog box procedure, see DialogProc.
		HWND hwnd = CreateDialogA( hInstance, lpName, hWndParent, lpDialogFunc );
		assert( hwnd );
		if ( hWndParent )
		{
			m_childHandles.push_back( hwnd );
		}
		ShowWindow( hwnd, SW_SHOW );
    }
    else
    {
        // Regardless of windowed or not, we need a window...
	    WNDCLASS wc;
	    memset( &wc, 0, sizeof( wc ) );
	    wc.style = 0;
	    wc.lpfnWndProc = (WNDPROC) WndProc;
	    wc.cbClsExtra = 0;
	    wc.cbWndExtra = 0;
	    wc.hInstance = GetHINSTANCE();
	    wc.hIcon = LoadIcon( (HINSTANCE) NULL, IDI_APPLICATION );
	    wc.hCursor = LoadCursor( (HINSTANCE) NULL, IDC_ARROW );

		if ( isPrimary )
		{
			wc.lpszMenuName =  L"MainMenu"; 
			wc.lpszClassName = L"MainWndClass"; 
		}
		else		    // TODO: This worked, yet is dodgy...
		{
			wc.lpszMenuName = 0;
			wc.lpszClassName = L"SecondWndClass";
		}
     
	    if ( !RegisterClass( &wc ) )
	    {
		    throw std::exception( "Failed to register window class!" );
	    }

        int x = static_cast< int >( details.GetPosition().x );
        int y = static_cast< int >( details.GetPosition().y );
        int width = static_cast< int >( details.GetSize().width );
        int height = static_cast< int >( details.GetSize().height );
        HWND parentHandle = details.GetParentHandle();
        HWND handle = CreateWindowA( "MainWndClass", "An Empty DirectX Project", WS_OVERLAPPEDWINDOW, x, y, width, height, 
            parentHandle, (HMENU) NULL, GetHINSTANCE(), (LPVOID) NULL); 
     
        if ( !handle )
        {
	        throw std::exception( "Failed to create window!" );
        }

		actual.SetHandle( handle );

        { // Resize window to ensure exact pixel match...
            RECT windowRect;
            GetWindowRect( actual.GetHandle(), &windowRect );

            RECT clientRect;
            GetClientRect( actual.GetHandle(), &clientRect );
        	
            long newWindowWidth = (windowRect.right - windowRect.left) + width - clientRect.right;
            long newWindowHeight = (windowRect.bottom - windowRect.top) + height - clientRect.bottom;
            MoveWindow( actual.GetHandle(), windowRect.left, windowRect.top, newWindowWidth, newWindowHeight, false );
        }

        ShowWindow( actual.GetHandle(), m_cmdShow );
        UpdateWindow( actual.GetHandle() );

		D3DPRESENT_PARAMETERS pp{};        
        pp.BackBufferWidth = width;
        pp.BackBufferHeight = height;
        pp.BackBufferFormat = D3DFMT_X8R8G8B8;
        pp.BackBufferCount = 1;
        pp.MultiSampleType = D3DMULTISAMPLE_NONE;
        pp.MultiSampleQuality = 0;
        pp.SwapEffect = D3DSWAPEFFECT_DISCARD;
        pp.hDeviceWindow = actual.GetHandle();
        pp.Windowed = details.IsFullscreen() ? 0 : 1;
        pp.EnableAutoDepthStencil = true;
        pp.AutoDepthStencilFormat = D3DFMT_D16; // D3DFMT_D24S8
        pp.Flags = 0;
    	
        if( details.IsFullscreen() )
        {
            pp.FullScreen_RefreshRateInHz = 60;
	        pp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
        }

		actual.SetPP( pp );

        IDirect3D9 * dx = Direct3DCreate9( D3D_SDK_VERSION );
        if ( ! dx )
        {
	        throw std::exception( "Failed to create DX!" );
        }
        
        D3DDEVTYPE deviceType = m_hardwareAcceleration ? D3DDEVTYPE_HAL : D3DDEVTYPE_REF;
        unsigned int behaviorFlags = 0;
        behaviorFlags |= m_hardwareAcceleration ? D3DCREATE_HARDWARE_VERTEXPROCESSING : D3DCREATE_SOFTWARE_VERTEXPROCESSING;

        HRESULT hr = S_OK;

		if ( m_dxDevice == 0 )
		{
			hr = dx->CreateDevice( 0, deviceType, actual.GetHandle(), behaviorFlags, &pp, &m_dxDevice );
			if ( FAILED( hr ) )
			{
				// Direct-X attempts to fix the presentation parameters, so a second attempt might resolve.
				hr = dx->CreateDevice( 0, deviceType, actual.GetHandle(), behaviorFlags, &pp, &m_dxDevice );
				if ( FAILED( hr ) )
				{
					throw std::exception( "Failed to create Direct-X device!" );
				}
			}

			IDirect3DSwapChain9 * swapChain{};
			m_dxDevice->GetSwapChain( 0,  &swapChain );
			actual.SetSwapChain( swapChain );
		}
		else
		{
			IDirect3DSwapChain9 * swapChain {};
			hr = m_dxDevice->CreateAdditionalSwapChain( &pp, &swapChain );
			if( FAILED( hr ) )
			{
				throw std::exception( "Failed to create Direct-X swap chain!" );
			}
			actual.SetSwapChain( swapChain );
		}
		actual.SetDxDevice( m_dxDevice );

        dx->Release();
        dx = 0;


	    // This is sort of a hack, to get ANY Direct-X Device (thus, we'll just grab the first).
	    // I believe the way Direct-X WANTS to work now is that resources are shared across Direct-X Devices,
	    // so we should be able to use ANY for most purposes.
	    if ( ! m_dxDevice && actual.GetDxDevice() )
	    {
	    }
	    
	    // Set our view matrix...
        D3DXMATRIX finalMatrix;

		// TODO: Hack
		float m_nearZ = 0.0f;
		float m_farZ = 1000.0f;
        D3DXMatrixOrthoOffCenterLH( &finalMatrix, 0, static_cast< float >( width ), static_cast< float >( height ), 0, m_nearZ, m_farZ );
		m_dxDevice->SetTransform( D3DTS_PROJECTION, &finalMatrix );
    }

    m_actualDisplays.push_back( std::pair< DisplayDetails, DisplayActual >( details, actual ) );
}

void TesterBase::DestroyDirectX()
{
	m_dxDevice = 0;
    for ( std::vector< std::pair< DisplayDetails, DisplayActual > >::iterator itr = m_actualDisplays.begin(), end = m_actualDisplays.end(); itr != end; ++itr )
    {
		/*
        IDirect3DDevice9 * & dxDevice = itr->second.dxDevice;
	    if ( dxDevice )
	    {
		    dxDevice->Release();
		    dxDevice = 0;
	    }
		*/
    }
}

void TesterBase::Render( IDirect3DDevice9 * dxDevice, int displayIndex )
{
    dxDevice; // NOT USED
}

size_t TesterBase::DisplayCount() const
{
    return m_actualDisplays.size();
}

const std::pair< DisplayDetails, DisplayActual > & TesterBase::GetDisplay( size_t index ) const
{
    assert( m_actualDisplays.size() > index );
    return m_actualDisplays[ index ];
}

std::pair< DisplayDetails, DisplayActual > & TesterBase::GetDisplay( size_t index )
{
	assert( m_actualDisplays.size() > index );
	return m_actualDisplays[index];
}

bool TesterBase::GetDisplayByHWND( HWND hwnd, size_t & indexOut )
{
	size_t i = 0;
    for ( std::vector< std::pair< DisplayDetails, DisplayActual > >::iterator itr = m_actualDisplays.begin(), end = m_actualDisplays.end(); itr != end; ++itr )
    {
		if ( itr->second.GetHandle() == hwnd )
		{
			indexOut = i;
			return true;
		}
		++i;
	}
	return false;
}

void TesterBase::RequestQuit()
{
	m_isQuitting = true;
}

HWND TesterBase::GetHWND() const
{
	return m_hwnd;
}

HINSTANCE TesterBase::GetHINSTANCE() const
{
    return m_hInstance;
}

bool TesterBase::IsQuitting() const
{
	return m_isQuitting;
}

Input & TesterBase::GetInput()
{
	return m_input;
}

/*
IDirect3DDevice9 * TesterBase::GetDxDevice()
{
	return m_dxDevice;
}
*/

TesterBase * TesterBase::s_tester;

TesterBase * TesterBase::GetInstance()
{
	return s_tester;
}