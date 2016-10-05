#include <Input.h>
#include <assert.h>

InputLock::InputLock()
: m_locked( false )
{
}

InputLock::~InputLock()
{
}

bool InputLock::Locked() const
{
	return m_locked;
}

void InputLock::SetLocked( bool locked )
{
	m_locked = locked;
}



Input::Input()
: m_shift( false )
, m_ctrl( false )
, m_alt( false )
, m_keys( 65536, 0 )
, m_mousePosition( 0, 0 )
, m_leftMouse( false )
, m_rightMouse( false )
, m_lock( 0 )
, m_displayIndex( 0 )
{
	for( size_t k = 0; k < 65536; ++k )
	{
		m_keys[ k ] = 0;
	}
}

Input::~Input()
{
}

bool Input::IsShift() const
{
	return m_shift;
}

bool Input::IsCtrl() const
{
	return m_ctrl;
}

bool Input::IsAlt() const
{
	return m_alt;
}

bool Input::IsKeyDown( unsigned int key ) const
{
	return m_keys[ key ] == 1;
}

bool Input::WasKeyPressed( unsigned int key ) const
{
	return m_keys[ key ] == 2;
}

V2 Input::GetMousePosition() const
{
	return m_mousePosition;
}

bool Input::IsLeftMouseDown() const
{
	return m_leftMouse == 1 ? true : false;
}

bool Input::IsRightMouseDown() const
{
	return m_rightMouse == 1 ? true : false;
}

bool Input::WasLeftMousePressed() const
{
	return m_leftMouse == 2 ? true : false;
}

bool Input::WasRightMousePressed() const
{
	return m_rightMouse == 2 ? true : false;
}

void Input::SetKey( unsigned int key, bool down )
{
	m_keys[ key ] = down ? 1 : 2;
}

void Input::SetShift( bool down )
{
	m_shift = down;
}

void Input::SetCtrl( bool down )
{
	m_ctrl = down;
}

void Input::SetAlt( bool down )
{
	m_alt = down;
}

void Input::SetMousePosition( size_t displayIndex, V2 position )
{
	m_displayIndex = displayIndex;
	m_mousePosition = position;
}

void Input::SetLeftMouse( bool down )
{
	m_leftMouse = down ? 1 : 2;
}

void Input::SetRightMouse( bool down )
{
	m_rightMouse = down ? 1 : 2;
}

void Input::CallAfterUpdate()
{
	// Clear pressed to up status...

	for( size_t k = 0; k < 65536; ++k )
	{
		if ( m_keys[ k ] == 2 )
		{
			m_keys[ k ] = 0;
		}
	}

	if ( m_leftMouse == 2 )
	{
		m_leftMouse = 0;
	}

	if ( m_rightMouse == 2 )
	{
		m_rightMouse = 0;
	}
}


bool Input::Locked() const
{
	return m_lock ? true : false;
}

bool Input::Lock( InputLock & lock )
{
	if ( Locked() )
	{
		return false;
	}

	m_lock = &lock;
	m_lock->SetLocked( true );
	return true;
}

void Input::Unlock( InputLock & lock )
{
	assert( m_lock == &lock );

	m_lock->SetLocked( false );
	m_lock = 0;
}
