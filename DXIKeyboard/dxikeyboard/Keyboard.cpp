// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxikeyboard/Keyboard.h>
#include <dxi/win/WindowsOS.h>
#include <dxi/exception/DeviceFailure.h>
#include <dinput.h>

using namespace dxikeyboard;
using namespace dxi;
using namespace input;

  
std::map< std::string, int, unify::CaseInsensitiveLessThanTest > g_KeyNameToIndex =
{
{"0", DIK_0 },
{ "1", DIK_1 },
{ "2", DIK_2 },
{ "3", DIK_3 },
{ "4", DIK_4 },
{ "5", DIK_5 },
{ "6", DIK_6 },
{ "7", DIK_7 },
{ "8", DIK_8 },
{ "9", DIK_9 },
{ "A", DIK_A },
{ "ABNT_C1", DIK_ABNT_C1 },
{ "ABNT_C2", DIK_ABNT_C2 },
{ "ADD", DIK_ADD },
{ "APOSTROPHE", DIK_APOSTROPHE },
{ "APPS", DIK_APPS },
{ "AT", DIK_AT },
{ "AX", DIK_AX },
{ "B", DIK_B },
{ "BACK", DIK_BACK },
{ "BACKSLASH", DIK_BACKSLASH },
{ "C", DIK_C },
{ "CALCULATOR", DIK_CALCULATOR },
{ "CAPITAL", DIK_CAPITAL },
{ "COLON", DIK_COLON },
{ "COMMA", DIK_COMMA },
{ "CONVERT", DIK_CONVERT },
{ "D", DIK_D },
{ "DECIMAL", DIK_DECIMAL },
{ "DELETE", DIK_DELETE },
{ "DIVIDE", DIK_DIVIDE },
{ "DOWN", DIK_DOWN },
{ "E", DIK_E },
{ "END", DIK_END },
{ "EQUALS", DIK_EQUALS },
{ "ESCAPE", DIK_ESCAPE },
{ "F", DIK_F },
{ "F1", DIK_F1 },
{ "F2", DIK_F2 },
{ "F3", DIK_F3 },
{ "F4", DIK_F4 },
{ "F5", DIK_F5 },
{ "F6", DIK_F6 },
{ "F7", DIK_F7 },
{ "F8", DIK_F8 },
{ "F9", DIK_F9 },
{ "F10", DIK_F10 },
{ "F11", DIK_F11 },
{ "F12", DIK_F12 },
{ "F13", DIK_F13 },
{ "F14", DIK_F14 },
{ "F15", DIK_F15 },
{ "G", DIK_G },
{ "GRAVE", DIK_GRAVE },
{ "H", DIK_H },
{ "HOME", DIK_HOME },
{ "I", DIK_I },
{ "INSERT", DIK_INSERT },
{ "J", DIK_J },
{ "K", DIK_K },
{ "KANA", DIK_KANA },
{ "KANJI", DIK_KANJI },
{ "L", DIK_L },
{ "LBRACKET", DIK_LBRACKET },
{ "LCONTROL", DIK_LCONTROL },
{ "LEFT", DIK_LEFT },
{ "LMENU", DIK_LMENU },
{ "LSHIFT", DIK_LSHIFT },
{ "LWIN", DIK_LWIN },
{ "M", DIK_M },
{ "MAIL", DIK_MAIL },
{ "MEDIASELECT", DIK_MEDIASELECT },
{ "MEDIASTOP", DIK_MEDIASTOP },
{ "MINUS", DIK_MINUS },
{ "MULTIPLY", DIK_MULTIPLY },
{ "MUTE", DIK_MUTE },
{ "MYCOMPUTER", DIK_MYCOMPUTER },
{ "N", DIK_N },
{ "NEXT", DIK_NEXT },
{ "NEXTTRACK", DIK_NEXTTRACK },
{ "NOCONVERT", DIK_NOCONVERT },
{ "NUMLOCK", DIK_NUMLOCK },
{ "NUMPAD0", DIK_NUMPAD0 },
{ "NUMPAD1", DIK_NUMPAD1 },
{ "NUMPAD2", DIK_NUMPAD2 },
{ "NUMPAD3", DIK_NUMPAD3 },
{ "NUMPAD4", DIK_NUMPAD4 },
{ "NUMPAD5", DIK_NUMPAD5 },
{ "NUMPAD6", DIK_NUMPAD6 },
{ "NUMPAD7", DIK_NUMPAD7 },
{ "NUMPAD8", DIK_NUMPAD8 },
{ "NUMPAD9", DIK_NUMPAD9 },
{ "NUMPADCOMMA", DIK_NUMPADCOMMA },
{ "NUMPADENTER", DIK_NUMPADENTER },
{ "NUMPADEQUALS", DIK_NUMPADEQUALS },
{ "O", DIK_O },
{ "OEM_102", DIK_OEM_102 },
{ "P", DIK_P },
{ "PAUSE", DIK_PAUSE },
{ "PERIOD", DIK_PERIOD },
{ "PLAYPAUSE", DIK_PLAYPAUSE },
{ "POWER", DIK_POWER },
{ "PREVTRACK", DIK_PREVTRACK },
{ "PRIOR", DIK_PRIOR },
{ "Q", DIK_Q },
{ "R", DIK_R },
{ "RBRACKET", DIK_RBRACKET },
{ "RCONTROL", DIK_RCONTROL },
{ "RETURN", DIK_RETURN },
{ "RIGHT", DIK_RIGHT },
{ "RMENU", DIK_RMENU },
{ "RSHIFT", DIK_RSHIFT },
{ "RWIN", DIK_RWIN },
{ "S", DIK_S },
{ "SCROLL", DIK_SCROLL },
{ "SEMICOLON", DIK_SEMICOLON },
{ "SLASH", DIK_SLASH },
{ "SLEEP", DIK_SLEEP },
{ "SPACE", DIK_SPACE },
{ "STOP", DIK_STOP },
{ "SUBTRACT", DIK_SUBTRACT },
{ "SYSRQ", DIK_SYSRQ },
{ "T", DIK_T },
{ "TAB", DIK_TAB },
{ "U", DIK_U },
{ "UNDERLINE", DIK_UNDERLINE },
{ "UNLABELED", DIK_UNLABELED },
{ "UP", DIK_UP },
{ "V", DIK_V },
{ "VOLUMEDOWN", DIK_VOLUMEDOWN },
{ "VOLUMEUP", DIK_VOLUMEUP },
{ "W", DIK_W },
{ "WAKE", DIK_WAKE },
{ "WEBBACK", DIK_WEBBACK },
{ "WEBFAVORITES", DIK_WEBFAVORITES },
{ "WEBFORWARD", DIK_WEBFORWARD },
{ "WEBHOME", DIK_WEBHOME },
{ "WEBREFRESH", DIK_WEBREFRESH },
{ "WEBSEARCH", DIK_WEBSEARCH },
{ "WEBSTOP", DIK_WEBSTOP },
{ "X", DIK_X },
{ "Y", DIK_Y },
{ "YEN", DIK_YEN },
{ "Z", DIK_Z },
{ "BACKSPACE", DIK_BACKSPACE },
{ "CAPSLOCK", DIK_CAPSLOCK },
{ "CIRCUMFLEX", DIK_CIRCUMFLEX },
{ "DOWNARROW", DIK_DOWNARROW },
{ "LALT", DIK_LALT },
{ "LEFTARROW", DIK_LEFTARROW },
{ "NUMPADMINUS", DIK_NUMPADMINUS },
{ "NUMPADPERIOD", DIK_NUMPADPERIOD },
{ "NUMPADPLUS", DIK_NUMPADPLUS },
{ "NUMPADSLASH", DIK_NUMPADSLASH },
{ "NUMPADSTAR", DIK_NUMPADSTAR },
{ "PGDN", DIK_PGDN },
{ "PGUP", DIK_PGUP },
{ "RALT", DIK_RALT },
{ "RIGHTARROW", DIK_RIGHTARROW },
{ "UPARROW", DIK_UPARROW }
};

std::map< int, std::string > g_KeyIndexToName =
{
	{ DIK_0, "0" },
	{ DIK_1, "1" },
	{ DIK_2, "2" },
	{ DIK_3, "3" },
	{ DIK_4, "4" },
	{ DIK_5, "5" },
	{ DIK_6, "6" },
	{ DIK_7, "7" },
	{ DIK_8, "8" },
	{ DIK_9, "9" },
	{ DIK_A, "A" },
	{ DIK_ABNT_C1, "ABNT_C1" },
	{ DIK_ABNT_C2, "ABNT_C2" },
	{ DIK_ADD, "ADD" },
	{ DIK_APOSTROPHE, "APOSTROPHE" },
	{ DIK_APPS, "APPS" },
	{ DIK_AT, "AT" },
	{ DIK_AX, "AX" },
	{ DIK_B, "B" },
	{ DIK_BACK, "BACK" },
	{ DIK_BACKSLASH, "BACKSLASH" },
	{ DIK_C, "C" },
	{ DIK_CALCULATOR, "CALCULATOR" },
	{ DIK_CAPITAL, "CAPITAL" },
	{ DIK_COLON, "COLON" },
	{ DIK_COMMA, "COMMA" },
	{ DIK_CONVERT, "CONVERT" },
	{ DIK_D, "D" },
	{ DIK_DECIMAL, "DECIMAL" },
	{ DIK_DELETE, "DELETE" },
	{ DIK_DIVIDE, "DIVIDE" },
	{ DIK_DOWN, "DOWN" },
	{ DIK_E, "E" },
	{ DIK_END, "END" },
	{ DIK_EQUALS, "EQUALS" },
	{ DIK_ESCAPE, "ESCAPE" },
	{ DIK_F, "F" },
	{ DIK_F1, "F1" },
	{ DIK_F2, "F2" },
	{ DIK_F3, "F3" },
	{ DIK_F4, "F4" },
	{ DIK_F5, "F5" },
	{ DIK_F6, "F6" },
	{ DIK_F7, "F7" },
	{ DIK_F8, "F8" },
	{ DIK_F9, "F9" },
	{ DIK_F10, "F10" },
	{ DIK_F11, "F11" },
	{ DIK_F12, "F12" },
	{ DIK_F13, "F13" },
	{ DIK_F14, "F14" },
	{ DIK_F15, "F15" },
	{ DIK_G, "G" },
	{ DIK_GRAVE, "GRAVE" },
	{ DIK_H, "H" },
	{ DIK_HOME, "HOME" },
	{ DIK_I, "I" },
	{ DIK_INSERT, "INSERT" },
	{ DIK_J, "J" },
	{ DIK_K, "K" },
	{ DIK_KANA, "KANA" },
	{ DIK_KANJI, "KANJI" },
	{ DIK_L, "L" },
	{ DIK_LBRACKET, "LBRACKET" },
	{ DIK_LCONTROL, "LCONTROL" },
	{ DIK_LEFT, "LEFT" },
	{ DIK_LMENU, "LMENU" },
	{ DIK_LSHIFT, "LSHIFT" },
	{ DIK_LWIN, "LWIN" },
	{ DIK_M, "M" },
	{ DIK_MAIL, "MAIL" },
	{ DIK_MEDIASELECT, "MEDIASELECT" },
	{ DIK_MEDIASTOP, "MEDIASTOP" },
	{ DIK_MINUS, "MINUS" },
	{ DIK_MULTIPLY, "MULTIPLY" },
	{ DIK_MUTE, "MUTE" },
	{ DIK_MYCOMPUTER, "MYCOMPUTER" },
	{ DIK_N, "N" },
	{ DIK_NEXT, "NEXT" },
	{ DIK_NEXTTRACK, "NEXTTRACK" },
	{ DIK_NOCONVERT, "NOCONVERT" },
	{ DIK_NUMLOCK, "NUMLOCK" },
	{ DIK_NUMPAD0, "NUMPAD0" },
	{ DIK_NUMPAD1, "NUMPAD1" },
	{ DIK_NUMPAD2, "NUMPAD2" },
	{ DIK_NUMPAD3, "NUMPAD3" },
	{ DIK_NUMPAD4, "NUMPAD4" },
	{ DIK_NUMPAD5, "NUMPAD5" },
	{ DIK_NUMPAD6, "NUMPAD6" },
	{ DIK_NUMPAD7, "NUMPAD7" },
	{ DIK_NUMPAD8, "NUMPAD8" },
	{ DIK_NUMPAD9, "NUMPAD9" },
	{ DIK_NUMPADCOMMA, "NUMPADCOMMA" },
	{ DIK_NUMPADENTER, "NUMPADENTER" },
	{ DIK_NUMPADEQUALS, "NUMPADEQUALS" },
	{ DIK_O, "O" },
	{ DIK_OEM_102, "OEM_102" },
	{ DIK_P, "P" },
	{ DIK_PAUSE, "PAUSE" },
	{ DIK_PERIOD, "PERIOD" },
	{ DIK_PLAYPAUSE, "PLAYPAUSE" },
	{ DIK_POWER, "POWER" },
	{ DIK_PREVTRACK, "PREVTRACK" },
	{ DIK_PRIOR, "PRIOR" },
	{ DIK_Q, "Q" },
	{ DIK_R, "R" },
	{ DIK_RBRACKET, "RBRACKET" },
	{ DIK_RCONTROL, "RCONTROL" },
	{ DIK_RETURN, "RETURN" },
	{ DIK_RIGHT, "RIGHT" },
	{ DIK_RMENU, "RMENU" },
	{ DIK_RSHIFT, "RSHIFT" },
	{ DIK_RWIN, "RWIN" },
	{ DIK_S, "S" },
	{ DIK_SCROLL, "SCROLL" },
	{ DIK_SEMICOLON, "SEMICOLON" },
	{ DIK_SLASH, "SLASH" },
	{ DIK_SLEEP, "SLEEP" },
	{ DIK_SPACE, "SPACE" },
	{ DIK_STOP, "STOP" },
	{ DIK_SUBTRACT, "SUBTRACT" },
	{ DIK_SYSRQ, "SYSRQ" },
	{ DIK_T, "T" },
	{ DIK_TAB, "TAB" },
	{ DIK_U, "U" },
	{ DIK_UNDERLINE, "UNDERLINE" },
	{ DIK_UNLABELED, "UNLABELED" },
	{ DIK_UP, "UP" },
	{ DIK_V, "V" },
	{ DIK_VOLUMEDOWN, "VOLUMEDOWN" },
	{ DIK_VOLUMEUP, "VOLUMEUP" },
	{ DIK_W, "W" },
	{ DIK_WAKE, "WAKE" },
	{ DIK_WEBBACK, "WEBBACK" },
	{ DIK_WEBFAVORITES, "WEBFAVORITES" },
	{ DIK_WEBFORWARD, "WEBFORWARD" },
	{ DIK_WEBHOME, "WEBHOME" },
	{ DIK_WEBREFRESH, "WEBREFRESH" },
	{ DIK_WEBSEARCH, "WEBSEARCH" },
	{ DIK_WEBSTOP, "WEBSTOP" },
	{ DIK_X, "X" },
	{ DIK_Y, "Y" },
	{ DIK_YEN, "YEN" },
	{ DIK_Z, "Z" },
	{ DIK_BACKSPACE, "BACKSPACE" },
	{ DIK_CAPSLOCK, "CAPSLOCK" },
	{ DIK_CIRCUMFLEX, "CIRCUMFLEX" },
	{ DIK_DOWNARROW, "DOWNARROW" },
	{ DIK_LALT, "LALT" },
	{ DIK_LEFTARROW, "LEFTARROW" },
	{ DIK_NUMPADMINUS, "NUMPADMINUS" },
	{ DIK_NUMPADPERIOD, "NUMPADPERIOD" },
	{ DIK_NUMPADPLUS, "NUMPADPLUS" },
	{ DIK_NUMPADSLASH, "NUMPADSLASH" },
	{ DIK_NUMPADSTAR, "NUMPADSTAR" },
	{ DIK_PGDN, "PGDN" },
	{ DIK_PGUP, "PGUP" },
	{ DIK_RALT, "RALT" },
	{ DIK_RIGHTARROW, "RIGHTARROW" },
	{ DIK_UPARROW, "UPARROW" }
};

Keyboard::Keyboard( dxi::core::Game * game )
	: m_game( game )
	, m_pdi( 0 )
	, m_pdiKeyboard( 0 )
	, m_iKeyDown( 0 )
	, m_hasFocus( false )
{
	// Set input buffers to defaults...
	for ( size_t k = 0; k < 256; k++ )
	{
		m_iKeyPressTimes[k] = 0;
		m_KeyStatus[k] = KeyStatus::Up;
	}
	for ( size_t k = 0; k < KeyStatus::COUNT; k++ )
	{
		m_uKeyStatusCount[k] = 0;
	}
	Acquire();
}

Keyboard::~Keyboard()
{
	m_pdiKeyboard = 0;
	m_pdi = 0;
}

std::string Keyboard::Name() const
{
	return "Keyboard";
}

void Keyboard::Acquire()
{
	win::WindowsOS & windowsOS = dynamic_cast< win::WindowsOS & >(m_game->GetOS());
	HWND handle = windowsOS.GetHandle();

	// Create the Direct Input device...
	if ( ! m_pdi )
	{
		if ( FAILED( DirectInput8Create( windowsOS.GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (VOID**)&m_pdi, 0 ) ) )
		{
			throw dxi::exception::DeviceFailure( "DirectInput", "Couldn't Create a device!" );
		}
	}

	// Create the Direct Input Keyboard device...
	m_pdiKeyboard = 0;

	if ( FAILED( m_pdi->CreateDevice( GUID_SysKeyboard, &m_pdiKeyboard, 0 ) ) )
	{
		throw exception::DeviceFailure( "DirectInput.Keyboard", "Couldn't create device!" );
	}

	if ( m_pdiKeyboard->SetCooperativeLevel( handle, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE ) != DI_OK )
	{
		throw exception::DeviceFailure( "DirectInput.Keyboard", "Couldn't set cooperative level!" );
	}

	if ( m_pdiKeyboard->SetDataFormat( &c_dfDIKeyboard ) != DI_OK )
	{
		throw exception::DeviceFailure( "DirectInput.Keyboard", "Couldn't set data format!" );
	}

	if ( m_pdiKeyboard->Acquire() != DI_OK )
	{
		throw exception::DeviceFailure( "DirectInput.Keyboard", "Failed to acquire!" );
	}

}

void Keyboard::Update()
{
	auto & os = m_game->GetOS();

	if ( !m_pdi )
	{
		Acquire();
		return;
	}

	// Pole Keyboard...
	if ( m_pdiKeyboard->GetDeviceState( 256, m_diKeyState ) != DI_OK )
	{
		throw exception::DeviceFailure( "DirectInput.Keyboard", "Couldn't get device state!" );
	}

	unsigned int u;
	for ( u = 0; u < KeyStatus::COUNT; u++ )
	{
		m_uKeyStatusCount[u] = 0;
	}

	auto diKeyPressed = []( auto x ) { return  ((x & 0x80) != 0); };

	// Key Pressed...
	for ( int i = 0; i < 256; ++i )
	{
		// Check if a key is being pressed..
		if ( diKeyPressed( m_diKeyState[ i ] ) )
		{
			m_iKeyPressTimes[i]++;

			std::string keyName = g_KeyIndexToName[i];

			if ( m_KeyStatus[i] == KeyStatus::Up )
			{
				m_KeyStatus[i] = KeyStatus::JustDown;
			}
			else
			{
				m_KeyStatus[i] = KeyStatus::Down;
			}

			if ( m_iKeyPressTimes[i] >= 2 ) m_iKeyPressTimes[i] = 2;
		}
		else
		{
			if ( m_KeyStatus[i] == KeyStatus::Down || m_KeyStatus[i] == KeyStatus::JustDown )
			{
				m_KeyStatus[i] = KeyStatus::Pressed;
			}
			else
			{
				m_KeyStatus[i] = KeyStatus::Up;
			}

			// Check if a key is flaged as repeating, if it is and is currently not pressed, clear it
			m_iKeyPressTimes[i] = 0;
		}

		m_uKeyStatusCount[m_KeyStatus[i]]++;
	}

	// Capslock, Scrolllock, Numlock, Insert...
	m_capslock = GetKeyState( VK_CAPITAL ) & 0x0001;
	m_numlock = GetKeyState( VK_NUMLOCK ) & 0x0001;
	m_scrolllock = GetKeyState( VK_SCROLL ) & 0x0001;
	m_insert = GetKeyState( VK_INSERT ) & 0x0001;
}

size_t Keyboard::SubSourceCount() const
{
	return 1;
}

State Keyboard::GetState( size_t subSource, std::string name, std::string condition ) const
{
	if ( subSource > 0 ) return State::Invalid;


	if ( unify::StringIs( condition, "On" ) )
	{
		if ( unify::StringIs( name, "CapsLock" ) )
		{
			return m_capslock ? State::True : State::False;
		}

		if ( unify::StringIs( name, "NumLock" ) )
		{
			return m_numlock ? State::True : State::False;
		}

		if ( unify::StringIs( name, "ScrollLock" ) )
		{
			return m_scrolllock ? State::True : State::False;
		}

		if ( unify::StringIs( name, "Insert" ) )
		{
			return m_insert ? State::True : State::False;
		}
	}

	auto diKeyPressed = []( auto x ) { return  ((x & 0x80) != 0); };

	int key = g_KeyNameToIndex[ name ];

	int escape = DIK_ESCAPE;

	if ( unify::StringIs( condition, "Down" ) )
	{
		return diKeyPressed( m_diKeyState[key] ) ? State::True : State::False;
	}

	if ( unify::StringIs( condition, "Pressed" ) )
	{
		return ( m_KeyStatus[key] == KeyStatus::Pressed ) ? State::True : State::False;
	}

	if ( unify::StringIs( condition, "JustDown" ) )
	{
		return ( m_KeyStatus[key] == KeyStatus::JustDown ) ? State::True : State::False;
	}

	return State::Invalid;
}

bool Keyboard::HasValue( size_t subSource, std::string name ) const
{
	return false;
}

float Keyboard::GetValue( size_t subSource, std::string name ) const
{
	return 0.0f;
}

bool Keyboard::SetState( size_t subSource, std::string name, std::string condition, bool set )
{
	return false;
}

bool Keyboard::SetValue( size_t subSource, std::string name, float value )
{
	return true;
}

