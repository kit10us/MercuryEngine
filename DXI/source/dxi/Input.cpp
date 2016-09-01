// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxi/Input.h>
#include <dxi/win/WindowsOS.h> // Requires specific Windows OS features.
#include <dxi/exception/DeviceFailure.h>

using namespace dxi;

Input::Input( core::IOS & os )
: m_os( os )
, m_pdi( 0 )
, m_pdiKeyboard( 0 )
, m_pdiMouse( 0 )
, m_iKeyDown( 0 )
, m_hasFocus( false )
{
	// Set input buffers to defaults...
	int k;
	for( k = 0; k < 256; k++ )
	{
		m_iKeyPressTimes[k] = 0;
		m_KeyStatus[k] = STATUS_UP;
	}		
	for( k = 0; k < 4; k++ )
	{
		m_mouseStatus[k] = STATUS_UP;
	}
	for( k = 0; k < NUM_STATUS; k++ )
	{
		m_uKeyStatusCount[k] = 0;
		m_uMouseStatusCount[k] = 0;
	}

    m_mouse = unify::V3< int >::V3Zero();
	Acquire();
}

Input::~Input()
{
	Destroy();
}

void Input::Acquire()
{
	win::WindowsOS & windowsOS = dynamic_cast< win::WindowsOS & >( m_os );

	// Create the Direct Input device...
	if( ! m_pdi )
	{
		if( FAILED( DirectInput8Create( windowsOS.GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pdi, 0 ) ) )
		{
			throw exception::DeviceFailure( "DirectInput", "Couldn't Create a device!" );
		}
	}

	// Create the Direct Input Keyboard device...
	try
	{
		CreateKeyboardDevice( windowsOS.GetHInstance(), windowsOS.GetHWnd(), windowsOS.GetFullscreen() );
	}
	catch( ... )
	{
	}

	// Setup the Direct Input Mouse device...
	try 
	{ 
		CreateMouseDevice( windowsOS.GetHInstance(), windowsOS.GetHWnd(), windowsOS.GetFullscreen() );
	} 
	catch( ... )
	{
	}
}

void Input::CreateKeyboardDevice( HINSTANCE hInstance, HWND hWnd, bool fullScreen )
{
	ReleaseKeyboardDevice();

	if( FAILED( m_pdi->CreateDevice( GUID_SysKeyboard, &m_pdiKeyboard, 0 ) ) )
	{
		throw exception::DeviceFailure( "DirectInput.Keyboard", "Couldn't create device!" );
	}

	if( m_pdiKeyboard->SetCooperativeLevel( hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE) != DI_OK )
	{
		throw exception::DeviceFailure( "DirectInput.Keyboard", "Couldn't set cooperative level!" );
	}

	if( m_pdiKeyboard->SetDataFormat( &c_dfDIKeyboard ) != DI_OK )
	{
		throw exception::DeviceFailure( "DirectInput.Keyboard", "Couldn't set data format!" );
	}
	
	if( m_pdiKeyboard->Acquire() != DI_OK )
	{
		throw exception::DeviceFailure( "DirectInput.Keyboard", "Failed to acquire!" );
	}
}

void Input::CreateMouseDevice( HINSTANCE hInstance, HWND hWnd, bool fullScreen )
{
	ReleaseMouseDevice();

	if ( FAILED( m_pdi->CreateDevice( GUID_SysMouse, &m_pdiMouse, 0 ) ) )
	{
		throw exception::DeviceFailure( "DirectInput.Mouse", "Couldn't create device!" );
	}

	// DISCL_FOREGROUND | DISCL_EXCLUSIVE
	unsigned int dwMode = DISCL_FOREGROUND | DISCL_NONEXCLUSIVE;
	if( fullScreen )
	{
		dwMode = DISCL_FOREGROUND | DISCL_EXCLUSIVE;
	}

	if( m_pdiMouse->SetCooperativeLevel( hWnd, dwMode) != DI_OK )
	{
		throw exception::DeviceFailure( "DirectInput.Mouse", "Couldn't set cooperative level!" );
	}

	if( m_pdiMouse->SetDataFormat(&c_dfDIMouse) != DI_OK )
	{
		throw exception::DeviceFailure( "DirectInput.Mouse", "Couldn't set data format!" );
	}
	
	if( m_pdiMouse->Acquire() != DI_OK )
	{
		Destroy();
	}
}

void Input::Destroy()
{
	ReleaseMouseDevice();
	ReleaseKeyboardDevice();

	if( m_pdi )
	{
		m_pdi->Release();
		m_pdi = 0;
	}
}

void Input::ReleaseKeyboardDevice()
{
	if( m_pdiKeyboard )
	{
		m_pdiKeyboard->Unacquire();
		if( m_pdiKeyboard )
		{
			m_pdiKeyboard->Release();
			m_pdiKeyboard = 0;
		}
	}
}

void Input::ReleaseMouseDevice()
{
	if( m_pdiMouse )
	{
		m_pdiMouse->Unacquire();
		if( m_pdiMouse )
		{
			m_pdiMouse->Release();
			m_pdiMouse = 0;
		}
	}
}

bool Input::IsReady() const
{
	return m_pdi != 0 && m_os.GetHasFocus();
}

void Input::CallBeforeUpdate( const unify::Size< int > resolution, bool fullScreen )
{
	if( ! m_pdi )
	{
		Acquire();
		return;
	}

	// Pole Keyboard...
	if( m_pdiKeyboard->GetDeviceState( 256, m_diKeyState ) != DI_OK )
	{
		throw exception::DeviceFailure( "DirectInput.Keyboard", "Couldn't get device state!" );
	}

	// Pole Mouse...
	HRESULT hr = m_pdiMouse->GetDeviceState( sizeof(DIMOUSESTATE), &m_diMouseState );
	if( FAILED(hr) )
	{
		// if the mouse have moved focus..
		if( hr == DIERR_INPUTLOST )
		{
			// Reacquire the mouse
			while( hr == DIERR_INPUTLOST )
			{
				hr = m_pdiMouse->Acquire();
			}

			// Try to test the state again
			if( SUCCEEDED(hr) )
			{
				hr = m_pdiMouse->GetDeviceState( sizeof(DIMOUSESTATE), &m_diMouseState );
			}
			else
			{
				exception::DeviceFailure( "DirectInput.Mouse", "Couldn't get device state!" );
			}
		}
		else
		{
			// TODO: If this happens often, we might need to update ready to a per-input device scale. Was: return FALSE;
		}
	}

	unsigned int u;
	for( u = 0; u < NUM_STATUS; u++ )
	{
		m_uKeyStatusCount[u] = 0;
		m_uMouseStatusCount[u] = 0;
	}

	// Key Pressed...
	for( int ik = 0; ik < 256; ++ik )
	{
		// Check if a key is being pressed..
		Key::TYPE k = static_cast< Key::TYPE >( ik );
		if( KeyState(k) )
		{
			m_iKeyPressTimes[k]++;

			if( m_KeyStatus[k] == STATUS_UP )
			{
				m_KeyStatus[k] = STATUS_JUSTDOWN;
			}
			else
			{
				m_KeyStatus[k] = STATUS_DOWN;
			}
			
			if( m_iKeyPressTimes[k] >= 2 ) m_iKeyPressTimes[k] = 2;
		}
		else
		{
			if( m_KeyStatus[k] == STATUS_DOWN || m_KeyStatus[k] == STATUS_JUSTDOWN )
			{
				m_KeyStatus[k] = STATUS_PRESSED;
			}
			else
			{
				m_KeyStatus[k] = STATUS_UP;
			}
			
			// Check if a key is flaged as repeating, if it is and is currently not pressed, clear it
			m_iKeyPressTimes[k] = 0;
		}

		m_uKeyStatusCount[m_KeyStatus[k]]++;
	}

	// Mouse...
	for( int b = 0; b < 4; b++ )
	{
		if( m_diMouseState.rgbButtons[b] & 0x80 )
		{
			if( m_mouseStatus[b] == STATUS_UP )
			{
				m_mouseStatus[b] = STATUS_JUSTDOWN;
			}
			else
			{
				m_mouseStatus[b] = STATUS_DOWN;
			}			
		}
		else
		{
			if( m_mouseStatus[b] == STATUS_DOWN || m_mouseStatus[b] == STATUS_JUSTDOWN )
			{
				m_mouseStatus[b] = STATUS_PRESSED;
			}
			else
			{
				m_mouseStatus[b] = STATUS_UP;
			}		
		}
		m_uMouseStatusCount[m_mouseStatus[b]]++;
	}

    /*
    m_mouse += unify::V3< int >( m_diMouseState.lX, m_diMouseState.lY, m_diMouseState.lZ );

	if( m_mouse.x < 0 ) m_mouse.x = 0;
	if( m_mouse.x >= resolution.width )
	{
		m_mouse.x = resolution.width - 1;
	}

	if( m_mouse.y < 0 ) m_mouse.y = 0;
	if( m_mouse.y >= resolution.height )
	{
		m_mouse.y = resolution.height - 1;
	}
    */
    m_mouse.z += m_diMouseState.lZ;

	// Capslock, Scrolllock, Numlock, Insert...
	m_bCapslock		= GetKeyState(VK_CAPITAL) & 0x0001;
	m_bNumlock		= GetKeyState(VK_NUMLOCK) & 0x0001;
	m_bScrolllock	= GetKeyState(VK_SCROLL) & 0x0001;
	m_bInsert		= GetKeyState(VK_INSERT) & 0x0001;
}

void Input::CallAfterUpdate()
{
}

void Input::SetMouseUnavailable()
{
    m_mouseAvailable = false;
}

void Input::SetMousePosition( unify::V2< int > position )
{
    m_mouseAvailable = true;
    m_mouse.x = position.x;
    m_mouse.y = position.y;
}

void Input::SetLeftMouse( bool down )
{
}

void Input::SetRightMouse( bool down )
{
}

bool Input::KeyState( Key::TYPE key ) const
{
	return IsReady() && diKeyPressed( m_diKeyState[ key ] );
}

bool Input::KeyDown( Key::TYPE key ) const
{
	return IsReady() && diKeyPressed( m_diKeyState[ key ] );
}

bool Input::KeyPressed( Key::TYPE key ) const
{
	return IsReady() && m_KeyStatus[ key ] == STATUS_PRESSED;
}

bool Input::KeyJustDown( Key::TYPE key ) const
{
	return IsReady() && m_KeyStatus[ key ] == STATUS_JUSTDOWN;
}

void Input::ClearKey( Key::TYPE key )
{
	m_uKeyStatusCount[ m_KeyStatus[ key ] ]--;
	m_KeyStatus[ key ] = STATUS_UP;
	m_uKeyStatusCount[ m_KeyStatus[ key ] ]++;
}

bool Input::AnyKeyStatus( InputStatus status )
{
	return IsReady() && m_uKeyStatusCount[status];
}

InputStatus Input::KeyStatus( Key::TYPE key ) const
{
	return m_KeyStatus[ key ];
}

bool Input::Capslock() const
{
	return m_bCapslock;
}

bool Input::Numlock() const
{
	return m_bNumlock;
}

bool Input::Scrolllock() const
{
	return m_bScrolllock;
}

bool Input::Insert() const
{
	return m_bInsert;
}


/////////////////////////////
// Mouse...
/////////////////////////////

bool Input::IsMouseAvailable() const
{
    return m_mouseAvailable;
}

int Input::MouseChangeX() const
{
	return( m_diMouseState.lX );
}

int Input::MouseChangeY() const
{
	return( m_diMouseState.lY );
}

int Input::MouseChangeZ() const
{
	return( m_diMouseState.lZ );
}

int Input::MouseX() const
{
	return( m_mouse.x );
}

int Input::MouseY() const
{
	return( m_mouse.y );
}

int Input::MouseZ() const
{
	return( m_mouse.z );
}

bool Input::MousePressed( int iButton ) const
{
	return( m_mouseStatus[iButton] == STATUS_PRESSED );
}

bool Input::MouseJustDown( int iButton ) const
{
	return( m_mouseStatus[iButton] == STATUS_JUSTDOWN );
}

bool Input::MouseDown( int iButton ) const
{
	if( m_diMouseState.rgbButtons[iButton] & 0x80 )
	{
		return true;
	}
	return FALSE;
}

// Same as mouse down
bool Input::MouseButton( int iButton )
{
	if( m_diMouseState.rgbButtons[iButton] & 0x80 )
	{
		return true;
	}
	return FALSE;
}

bool Input::AnyMouseStatus( InputStatus status )
{
	if( m_uMouseStatusCount[status] ) return true;
	return FALSE;
}


char dxi::GetDIKChar( int p_iKey, unsigned int dwShiftState )
{
	switch( dwShiftState )
	{
	case ( SHIFTSTATE_NONE ):
		switch( p_iKey )
		{
		case DIK_A:	return 'a';
		case DIK_B: return 'b';
		case DIK_C: return 'c';
		case DIK_D: return 'd';
		case DIK_E: return 'e';
		case DIK_F: return 'f';
		case DIK_G: return 'g';
		case DIK_H: return 'h';
		case DIK_I: return 'i';
		case DIK_J: return 'j';
		case DIK_K: return 'k';
		case DIK_L: return 'l';
		case DIK_M: return 'm';
		case DIK_N: return 'n';
		case DIK_O: return 'o';
		case DIK_P: return 'p';
		case DIK_Q: return 'q';
		case DIK_R: return 'r';
		case DIK_S: return 's';
		case DIK_T: return 't';
		case DIK_U: return 'u';
		case DIK_V: return 'v';
		case DIK_W: return 'w';
		case DIK_X: return 'x';
		case DIK_Y: return 'y';
		case DIK_Z: return 'z';
		};
		break;
	
	case ( SHIFTSTATE_CAPSLOCK | SHIFTSTATE_SHIFT ):
		switch (p_iKey) {
		case DIK_A:	return 'a';
		case DIK_B: return 'b';
		case DIK_C: return 'c';
		case DIK_D: return 'd';
		case DIK_E: return 'e';
		case DIK_F: return 'f';
		case DIK_G: return 'g';
		case DIK_H: return 'h';
		case DIK_I: return 'i';
		case DIK_J: return 'j';
		case DIK_K: return 'k';
		case DIK_L: return 'l';
		case DIK_M: return 'm';
		case DIK_N: return 'n';
		case DIK_O: return 'o';
		case DIK_P: return 'p';
		case DIK_Q: return 'q';
		case DIK_R: return 'r';
		case DIK_S: return 's';
		case DIK_T: return 't';
		case DIK_U: return 'u';
		case DIK_V: return 'v';
		case DIK_W: return 'w';
		case DIK_X: return 'x';
		case DIK_Y: return 'y';
		case DIK_Z: return 'z';

		case DIK_1: return '!';
		case DIK_2: return '@';
		case DIK_3: return '#';
		case DIK_4: return '$';
		case DIK_5: return '%';
		case DIK_6: return '^';
		case DIK_7: return '&';
		case DIK_8: return '*';
		case DIK_9: return '(';
		case DIK_0: return ')';

		case DIK_APOSTROPHE: return '"';
		case DIK_COMMA: return '<';	
		case DIK_EQUALS: return '+';
		case DIK_GRAVE: return '~';
		case DIK_LBRACKET: return '{';
		case DIK_MINUS: return '_';
		case DIK_PERIOD: return '>';

		case DIK_RBRACKET: return '}';
		case DIK_SLASH: return '?';

		case DIK_SEMICOLON: return ':';
		}
		break;

	case ( SHIFTSTATE_CAPSLOCK ): // Capslock (only affects letters)
		switch (p_iKey)
		{
		case DIK_A:	return 'A';
		case DIK_B: return 'B';
		case DIK_C: return 'C';
		case DIK_D: return 'D';
		case DIK_E: return 'E';
		case DIK_F: return 'F';
		case DIK_G: return 'G';
		case DIK_H: return 'H';
		case DIK_I: return 'I';
		case DIK_J: return 'J';
		case DIK_K: return 'K';
		case DIK_L: return 'L';
		case DIK_M: return 'M';
		case DIK_N: return 'N';
		case DIK_O: return 'O';
		case DIK_P: return 'P';
		case DIK_Q: return 'Q';
		case DIK_R: return 'R';
		case DIK_S: return 'S';
		case DIK_T: return 'T';
		case DIK_U: return 'U';
		case DIK_V: return 'V';
		case DIK_W: return 'W';
		case DIK_X: return 'X';
		case DIK_Y: return 'Y';
		case DIK_Z: return 'Z';
		}
		break;

	case ( SHIFTSTATE_SHIFT ): // Shift (affects almost all keys)
		switch (p_iKey)
		{
		case DIK_A:	return 'A';
		case DIK_B: return 'B';
		case DIK_C: return 'C';
		case DIK_D: return 'D';
		case DIK_E: return 'E';
		case DIK_F: return 'F';
		case DIK_G: return 'G';
		case DIK_H: return 'H';
		case DIK_I: return 'I';
		case DIK_J: return 'J';
		case DIK_K: return 'K';
		case DIK_L: return 'L';
		case DIK_M: return 'M';
		case DIK_N: return 'N';
		case DIK_O: return 'O';
		case DIK_P: return 'P';
		case DIK_Q: return 'Q';
		case DIK_R: return 'R';
		case DIK_S: return 'S';
		case DIK_T: return 'T';
		case DIK_U: return 'U';
		case DIK_V: return 'V';
		case DIK_W: return 'W';
		case DIK_X: return 'X';
		case DIK_Y: return 'Y';
		case DIK_Z: return 'Z';

		case DIK_1: return '!';
		case DIK_2: return '@';
		case DIK_3: return '#';
		case DIK_4: return '$';
		case DIK_5: return '%';
		case DIK_6: return '^';
		case DIK_7: return '&';
		case DIK_8: return '*';
		case DIK_9: return '(';
		case DIK_0: return ')';

		case DIK_APOSTROPHE: return '"';
		case DIK_COMMA: return '<';	
		case DIK_EQUALS: return '+';
		case DIK_GRAVE: return '~';
		case DIK_LBRACKET: return '{';
		case DIK_MINUS: return '_';
		case DIK_PERIOD: return '>';

		case DIK_RBRACKET: return '}';
		case DIK_SLASH: return '?';

		case DIK_SEMICOLON: return ':';
		}
		break;
	}

	// Normal (non-letters)...
	switch( p_iKey )
	{
	case DIK_1: return '1';
	case DIK_2: return '2';
	case DIK_3: return '3';
	case DIK_4: return '4';
	case DIK_5: return '5';
	case DIK_6: return '6';
	case DIK_7: return '7';
	case DIK_8: return '8';
	case DIK_9: return '9';
	case DIK_0: return '0';

	case DIK_NUMPAD0: return '0';
	case DIK_NUMPAD1: return '1';
	case DIK_NUMPAD2: return '2';
	case DIK_NUMPAD3: return '3';
	case DIK_NUMPAD4: return '4';
	case DIK_NUMPAD5: return '5';
	case DIK_NUMPAD6: return '6';
	case DIK_NUMPAD7: return '7';
	case DIK_NUMPAD8: return '8';
	case DIK_NUMPAD9: return '9';

	case DIK_SPACE: return ' ';
	case DIK_APOSTROPHE: return '\'';	
	case DIK_COMMA: return ',';	
	case DIK_EQUALS: return '=';
	case DIK_GRAVE: return '`';
	case DIK_LBRACKET: return '[';
	case DIK_MINUS: return '-';
	case DIK_PERIOD: return '.';
	case DIK_MULTIPLY: return '*';	
	case DIK_NUMPADCOMMA: return ',';

	case DIK_NUMPADEQUALS: return '=';
	case DIK_RBRACKET: return ']';
	case DIK_SLASH: return '/';
	case DIK_SUBTRACT: return '-';
	case DIK_UNDERLINE: return '_';
	case DIK_DECIMAL: return '.';

	case DIK_COLON: return ':';
	case DIK_SEMICOLON: return ';';
	}

	return NULL; // Unknown
}
