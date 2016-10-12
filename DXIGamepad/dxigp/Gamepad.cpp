// Copyright (c) 2003 - 2013, Quentin S. Smith
// All Rights Reserved

#include <dxigp/Gamepad.h>

#pragma comment( lib, "Xinput9_1_0.lib" )

using namespace dxigp;
using namespace dxi;

Gamepad::Gamepad( dxi::core::Game * game )
	: m_game( game )
{
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
	auto & os = m_game->GetOS();
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

input::State Gamepad::GetState( size_t subSource, std::string name, std::string condition ) const
{
	auto itr = m_states.find( subSource );
	if ( itr == m_states.end() )
	{
		return input::State::Invalid;
	}

	const XINPUT_STATE * prevState = 0;
	auto itrPrevious = m_prevStates.find( subSource );
	if ( itrPrevious != m_prevStates.end() )
	{
		prevState = &itrPrevious->second;
	}

	const XINPUT_STATE * state = &itr->second;

	WORD button = {};
	if ( unify::StringIs( name, "DPAD_UP" ) )
	{				 
		button = XINPUT_GAMEPAD_DPAD_UP;
	}
	else if ( unify::StringIs( name, "DPAD_DOWN" ) )
	{
		button = XINPUT_GAMEPAD_DPAD_DOWN;
	}
	else if ( unify::StringIs( name, "DPAD_LEFT" ) )
	{
		button = XINPUT_GAMEPAD_DPAD_LEFT;
	}
	else if ( unify::StringIs( name, "DPAD_RIGHT" ) )
	{
		button = XINPUT_GAMEPAD_DPAD_RIGHT;
	}
	else if ( unify::StringIs( name, "START" ) )
	{
		button = XINPUT_GAMEPAD_START;
	}
	else if ( unify::StringIs( name, "BACK" ) )
	{
		button = XINPUT_GAMEPAD_BACK;
	}
	else if ( unify::StringIs( name, "LEFT_THUMB" ) )
	{
		button = XINPUT_GAMEPAD_LEFT_THUMB;
	}
	else if ( unify::StringIs( name, "RIGHT_THUMB" ) )
	{
		button = XINPUT_GAMEPAD_RIGHT_THUMB;
	}
	else if ( unify::StringIs( name, "LEFT_SHOULDER" ) )
	{
		button = XINPUT_GAMEPAD_LEFT_SHOULDER;
	}
	else if ( unify::StringIs( name, "RIGHT_SHOULDER" ) )
	{
		button = XINPUT_GAMEPAD_RIGHT_SHOULDER;
	}
	else if ( unify::StringIs( name, "A" ) )
	{
		button = XINPUT_GAMEPAD_A;
	}
	else if ( unify::StringIs( name, "B" ) )
	{
		button = XINPUT_GAMEPAD_B;
	}
	else if ( unify::StringIs( name, "X" ) )
	{
		button = XINPUT_GAMEPAD_X;
	}
	else if ( unify::StringIs( name, "Y" ) )
	{
		button = XINPUT_GAMEPAD_Y;
	}

	if ( button )
	{
		if ( unify::StringIs( condition, "Down" ) )
		{
			return ( ( state->Gamepad.wButtons & button ) == button ) ? input::State::True : input::State::False;
		}
		else if ( unify::StringIs( condition, "Up" ) )
		{
			return ( (state->Gamepad.wButtons & button) == 0 ) ? input::State::True : input::State::False;
		}
		else if ( unify::StringIs( condition, "Pressed" ) )
		{
			if ( ! prevState )
			{
				return input::State::False;
			}

			return ( ( (state->Gamepad.wButtons & button) == 0 ) && ( ( prevState->Gamepad.wButtons & button ) == button ) ) ? input::State::True : input::State::False;
		}
	}

	return input::State::Invalid;
}

bool Gamepad::HasValue( size_t subSource, std::string name ) const
{	
	auto itr = m_states.find( subSource );
	if ( itr == m_states.end() )
	{
		return false;
	}				

	if ( unify::StringIs( name, "LEFTTRIGGER" ) )
	{
		return true;
	}
	else if ( unify::StringIs( name, "RIGHTTRIGGER" ) )
	{
		return true;	
	}
	else if ( unify::StringIs( name, "THUMBLX" ) )
	{
		return true;
	}
	else if ( unify::StringIs( name, "THUMBLY" ) )
	{
		return true;
	}
	else if ( unify::StringIs( name, "THUMBRX" ) )
	{
		return true;
	}
	else if ( unify::StringIs( name, "THUMBRY" ) )
	{
		return true;
	}

	return false;	
}

float Gamepad::GetValue( size_t subSource, std::string name ) const
{
	auto itr = m_states.find( subSource );
	if ( itr == m_states.end() )
	{
		return 0.0f;
	}

	auto ThumbValue = []( auto v ) { return (v < 0) ? (v / 32768.0f) : (v / 32767.0f); };
	auto TriggerValue = []( auto v ) { return (v <= XINPUT_GAMEPAD_TRIGGER_THRESHOLD) ? 0.0f : ( v / 255.f ); };

	const XINPUT_STATE * state = &itr->second;

	if ( unify::StringIs( name, "LEFTTRIGGER" ) )
	{
		return TriggerValue( state->Gamepad.bLeftTrigger );
	}
	else if ( unify::StringIs( name, "RIGHTTRIGGER" ) )
	{
		return TriggerValue( state->Gamepad.bRightTrigger );
	}
	else if ( unify::StringIs( name, "THUMBLX" ) )
	{
		return ThumbValue( state->Gamepad.sThumbLX );
	}
	else if ( unify::StringIs( name, "THUMBLY" ) )
	{
		return ThumbValue( state->Gamepad.sThumbLY );
	}
	else if ( unify::StringIs( name, "THUMBRX" ) )
	{
		return ThumbValue( state->Gamepad.sThumbRX );
	}
	else if ( unify::StringIs( name, "THUMBRY" ) )
	{
		return ThumbValue( state->Gamepad.sThumbRY );
	}

	return false;
}

bool Gamepad::SetState( size_t subSource, std::string name, std::string condition, bool set )
{
	return false;
}

bool Gamepad::SetValue( size_t subSource, std::string name, float value )
{
	return false;
}
