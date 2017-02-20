// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxigp/Gamepad.h>

#pragma comment( lib, "Xinput9_1_0.lib" )

using namespace dxigp;
using namespace me;
using namespace input;

std::vector< std::string > g_ButtonNames =
{
	"DPAD_UP",
	"DPAD_DOWN",
	"DPAD_LEFT",
	"DPAD_RIGHT",
	"START",
	"BACK",
	"LEFT_THUMB",
	"RIGHT_THUMB",
	"LEFT_SHOULDER",
	"RIGHT_SHOULDER",
	"A",
	"B",
	"X",
	"Y"
};

std::vector< WORD > g_ButtonCodes =
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT,
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y
};

std::vector< std::string > g_AnalogNames =
{
	"LEFTTRIGGER"
	"RIGHTTRIGGER",
	"THUMBLX",
	"THUMBLY",
	"THUMBRX",
	"THUMBRY"
};

Gamepad::Gamepad( me::IGame * game )
	: m_game( game )
{
	size_t index = 0;
	for ( auto && name : g_ButtonNames )
	{
		m_nameToIndex[ name ] = index++;
	}
	for ( auto && name : g_AnalogNames )
	{
		m_nameToIndex[ name ] = index++;
	}
}

Gamepad::~Gamepad()
{
}

std::string Gamepad::Name() const
{
	return "Gamepad";
}

void Gamepad::Update()
{
	m_prevStates = m_states;
	m_states.clear();
	for ( DWORD i = 0; i < XUSER_MAX_COUNT; ++i )
	{
		XINPUT_STATE state{};
		if ( XInputGetState( i, &state ) == ERROR_SUCCESS )
		{	  
			m_states[i] = state;
		}
	}
}

size_t Gamepad::SubSourceCount() const
{
	return m_states.size();
}

size_t Gamepad::InputCount( size_t subSource ) const
{
	return g_ButtonNames.size() + g_AnalogNames.size();
}

std::string Gamepad::InputName( size_t subSource, size_t index ) const
{
	if ( index < g_ButtonNames.size() )
	{
		return g_ButtonNames[ index ];
	}
	else if ( index < (g_ButtonNames.size() + g_AnalogNames.size()) )
	{
		return g_AnalogNames[ index - g_ButtonNames.size() ];
	}
	else
	{
		return std::string();
	}
}

size_t Gamepad::InputIndex( size_t subSource, std::string name ) const
{
	auto && itr = m_nameToIndex.find( name );
	if ( itr == m_nameToIndex.end() )
	{
		return MAXSIZE_T;
	}
	else
	{
		return itr->second;
	}
}

size_t Gamepad::InputConditionCount( size_t subSource, size_t index ) const
{
	if ( index < g_ButtonNames.size() )
	{
		return 3;
	}
	else if ( index < (g_ButtonNames.size() + g_AnalogNames.size()) )
	{
		return MAXSIZE_T;
	}
	else
	{
		return MAXSIZE_T;
	}
}

std::string Gamepad::InputConditionName( size_t subSource, size_t index, size_t condition ) const
{
	if ( index < g_ButtonNames.size() )
	{
		switch ( condition )
		{
		case 0:
			return "Down";
		case 1:
			return "Up";
		case 2:
			return "Pressed";
		default:
			return std::string();
		}
	}
	else if ( index < (g_ButtonNames.size() + g_AnalogNames.size()) )
	{
		return std::string();
	}
	else
	{
		return std::string();
	}
}

size_t Gamepad::InputConditionIndex( size_t subSource, size_t index, std::string condition ) const
{
	if ( index < g_ButtonNames.size() )
	{
		if ( unify::StringIs( condition, "Down" ) )
		{
			return 0;
		}
		else if ( unify::StringIs( condition, "Up" ) )
		{
			return 1;
		}
		else if ( unify::StringIs( condition, "Pressed" ) )
		{
			return 2;
		}
		else
		{
			return MAXSIZE_T;
		}
	}
	else if ( index < (g_ButtonNames.size() + g_AnalogNames.size()) )
	{
		return MAXSIZE_T;
	}
	else
	{
		return MAXSIZE_T;
	}
}

State Gamepad::GetState( size_t subSource, size_t index, size_t condition ) const
{
	auto itr = m_states.find( subSource );
	if ( itr == m_states.end() )
	{
		return State::Invalid;
	}

	const XINPUT_STATE * prevState = 0;
	auto itrPrevious = m_prevStates.find( subSource );
	if ( itrPrevious != m_prevStates.end() )
	{
		prevState = &itrPrevious->second;
	}

	const XINPUT_STATE * state = &itr->second;

	if ( index < g_ButtonCodes.size() )
	{
		WORD button = g_ButtonCodes[ index ];
		switch ( condition )
		{
		case 0:
			return ((state->Gamepad.wButtons & button) == button) ? State::True : State::False;
		case 1:
			return ((state->Gamepad.wButtons & button) == 0) ? State::True : State::False;
		case 3:
			if ( !prevState )
			{
				return State::False;
			}
			return (((state->Gamepad.wButtons & button) == 0) && ((prevState->Gamepad.wButtons & button) == button)) ? State::True : State::False;
		}
	}

	return State::Invalid;
}

bool Gamepad::HasValue( size_t subSource, size_t index ) const
{	
	auto itr = m_states.find( subSource );
	if ( itr == m_states.end() )
	{
		return false;
	}		

	if ( index < g_ButtonCodes.size() )
	{
		return false;
	}
	else if ( index < (g_ButtonCodes.size() + g_AnalogNames.size()) )
	{
		return true;
	}
	else
	{
		return false;
	}
}

float Gamepad::GetValue( size_t subSource, size_t index ) const
{
	auto itr = m_states.find( subSource );
	if ( itr == m_states.end() )
	{
		return 0.0f;
	}

	auto ThumbValue = []( auto v ) { return (v < 0) ? (v / 32768.0f) : (v / 32767.0f); };
	auto TriggerValue = []( auto v ) { return (v <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? 0.0f : ( v / 255.f ); };

	const XINPUT_STATE * state = &itr->second;

	if ( index < g_ButtonCodes.size() )
	{
		return 0.0f;
	}
	else if ( index < (g_ButtonCodes.size() + g_AnalogNames.size()) )
	{
		switch( index - g_ButtonCodes.size() )
		{
		case 0:
			return TriggerValue( state->Gamepad.bLeftTrigger );
		case 1:
			return TriggerValue( state->Gamepad.bRightTrigger );
		case 2:
			return ThumbValue( state->Gamepad.sThumbLX );
		case 3:
			return ThumbValue( state->Gamepad.sThumbLY );
		case 4:
			return ThumbValue( state->Gamepad.sThumbRX );
		case 5:
			return ThumbValue( state->Gamepad.sThumbRY );
		default:
			return 0.0f;
		}
	}
	else
	{
		return 0.0f;
	}
}

bool Gamepad::SetState( size_t subSource, size_t index, size_t condition, bool set )
{
	return false;
}

bool Gamepad::SetValue( size_t subSource, size_t index, float value )
{
	return false;
}
